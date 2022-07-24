// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDSimulation.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"

#include "VDCollisionInterface.h"
#include "BaseVDComponent.h"
#include "VDSimulationSettingsActor.h"
#include "VDWindDirectionalSourceComponent.h"
#include "HAL/RunnableThread.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "SceneInterface.h"

/* Thread Worker class */
FSimulationThreadWorker::FSimulationThreadWorker(UVDSimulation* InSimulation, uint32 CoreAffinity) : VDSimulation(InSimulation), FrameComplete(false)
{
#if PLATFORM_SWITCH
	if (CoreAffinity == 1)
	{
		UE_LOG(LogVICODynamics, All, TEXT("Running the Worker thread on core 1 results in crashes, please use either core 0 or core 2. Auto correcting to core 2..."));
		CoreAffinity = 2;
	}
#endif

	const uint64 Affinity = 1LL << static_cast<uint64>(CoreAffinity);
	Thread = FRunnableThread::Create(this, TEXT("VDSimulationWorkerThread"), 0, TPri_Normal, Affinity);
}

FSimulationThreadWorker::~FSimulationThreadWorker()
{
	VDSimulation = nullptr;
	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
}

bool FSimulationThreadWorker::Init()
{
	return VDSimulation && Thread;
}

uint32 FSimulationThreadWorker::Run()
{
	while (VDSimulation)
	{
		if (!FrameComplete)
		{
			SCOPE_CYCLE_COUNTER(STAT_VDSimWorkerTick);

			if (!WorldRef.IsValid())
			{
				FrameComplete = true;
				FPlatformProcess::Sleep(0);
				continue;
			}

			auto World = WorldRef.Get();
			if (World && !World->bIsTearingDown)
			{
				FScopeLock Lock(&VDSimulation->SimulationSettingsCritSec);

				auto CollisionInterface = VDSimulation->CollisionInterface;
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
				if (CollisionInterface && CollisionInterface->DrawCollisionDebug)
					FlushPersistentDebugLines(World);
#endif
				CollisionInterface->ResetCache();
				
				VDSimulation->Simulation->TickSimulation(World->DeltaTimeSeconds);
			}

			FrameComplete = true;
		}
		else
		{
			FPlatformProcess::Sleep(0);
		}
	}

	return 0;
}

void FSimulationThreadWorker::Stop()
{
	VDSimulation = nullptr;
}

void FSimulationThreadWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FSimulationThreadWorker::TerminateImmediately()
{
	Stop();
	Thread->Kill(false);
}

void FVDSimulationTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	Target->Tick(DeltaTime);
}

FString FVDSimulationTickFunction::DiagnosticMessage()
{
	static FString msg = TEXT("VICO Dynamics Simulation Tick");
	return msg;
}
/////////////////////////////////////////////////////////////////////////////////

/* VDSimulation class */
UVDSimulation::UVDSimulation()
{
	Simulation = nullptr;
	bTickRegistered = false;
	RunOnWorkerThread = false;
	bShowOnScreenStats = false;

	CurWorld = nullptr;
}

vicoDynamics::FSimulation* UVDSimulation::GetSimulation()
{
	if (Simulation) return Simulation;

	if (GWorld)
	{
		for (TActorIterator<AVDSimulationSettingsActor> It(GWorld, AVDSimulationSettingsActor::StaticClass()); It; ++It)
		{
			AVDSimulationSettingsActor* Actor = *It;
			if (!Actor->IsPendingKill())
			{
				Settings = Actor->GetSettings();
				break;
			}
		}
	}

	auto ConfigSettings = GetDefault<UVICODynamicsSettings>();
	if (ConfigSettings)
		Settings.CopyFrom(ConfigSettings);
	
	static const uint64_t Affinities[8] = { MAKEAFFINITYMASK1(0), MAKEAFFINITYMASK1(1), MAKEAFFINITYMASK1(2), MAKEAFFINITYMASK1(3), MAKEAFFINITYMASK1(4), MAKEAFFINITYMASK1(5), MAKEAFFINITYMASK1(6), MAKEAFFINITYMASK1(7) };
#if PLATFORM_SWITCH
	const static auto NUM_CORES = 3;
	Settings.NumberOfThreadsToUse = FMath::Min(Settings.NumberOfThreadsToUse, NUM_CORES);
#endif
	Simulation = CreateSimulation(Settings.ReserveParticleBudget, DEFAULT_BROADPHASE_MAX_PARTICLES_PER_CELL, Settings.NumberOfThreadsToUse, &Affinities[0]); // Thread pool threads, only reserved if using *_Parallel builds
	Simulation->SetSelfCollisionEnabled(Settings.EnableSelfCollision);
	Simulation->SetPhysXSceneCollisionEnabled(Settings.PerformExtraCollisionResolutionLoop);
	Simulation->SetSimulationTimeStep(1.f / static_cast<float>(Settings.SimulationFramerate));
	Simulation->SetSimulationIterations(Settings.SimulationIterations);
	Simulation->SetConstraintIterations(Settings.ConstraintIterations);

	RunOnWorkerThread = Settings.RunSimulationOnWorkerThread;

	const auto PhysicsScene = GWorld ? GWorld->GetPhysicsScene() : nullptr;
	if (PhysicsScene) // Enable Rigid body collisions
	{
		Simulation->SetPhysXScenes(PhysicsScene->GetPxScene(), /*PhysicsScene->HasAsyncScene() ? PhysicsScene->GetPhysXScene(EPhysicsSceneType::PST_Async) :*/ nullptr);
		Simulation->SetPhysXSceneCollisionEnabled(Settings.EnablePhysXSceneCollision);
		Simulation->SetCollisionQueryInterface(CollisionInterface = new FVDCollisionQueryInterface(this));
		ExplicitRigidBodyCollisionEnabled = Settings.EnableExplicitRigidBodyCollision;
	}

	if (CollisionInterface)
	{
		CollisionInterface->bCachePerFrameResults = Settings.bCachePerFrameResults;
		CollisionInterface->bCacheOnlyCollisionsWithStaticRigidBodies = Settings.bCacheOnlyCollisionsWithStaticRigidBodies;
		CollisionInterface->bCacheNonHitsAlso = Settings.bCacheNonHitsAlso;
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		CollisionInterface->DrawCollisionDebug = Settings.DrawCollisionDebug;	
#endif
	}

	bShowOnScreenStats = Settings.bShowOnScreenStats;
	
	checkf(Simulation, TEXT("Error occured while creating the Simulation..."));

	IVICODynamicsPlugin::Get().Released = false;

	return Simulation;
}

void UVDSimulation::Register(IVICODynamicsSimulated* Simulated)
{
	//checkf(Simulation, TEXT("No Simulation created, call GetSimulation before Registering"));
	checkf(Simulated, TEXT("Simulated object to register cannot be null"));

	RegisteredSimulatedObjects.AddUnique(Simulated);

	if (RegisteredSimulatedObjects.Num() == 1 && !bTickRegistered) // First Component, we can now register our Tick
	{
		CurWorld = GWorld.GetReference();

 		TickFunction.Target = this;
		TickFunction.TickGroup = ETickingGroup::TG_PostPhysics;
 		TickFunction.SetTickFunctionEnable(true);
 		TickFunction.RegisterTickFunction(CurWorld->PersistentLevel);
		bTickRegistered = true;
		
		if (RunOnWorkerThread && !SimulationThreadWorker.IsValid())
		{
			SimulationThreadWorker = MakeShareable<FSimulationThreadWorker>(new FSimulationThreadWorker(this, Settings.WorkerThreadCoreAffinity));
			SimulationThreadWorker->FrameComplete = true; // Don't try to tick until the World is ready.
			SimulationThreadWorker->SetWorldRef(CurWorld);
			bWorkerThreadNotifiedOfEndLevel = false;
		}

		if (Simulation)
			Simulation->SetGravity(vicoDynamics::FVector3Data(0.f, 0.f, CurWorld->GetGravityZ()));
	}
}

void UVDSimulation::Unregister(IVICODynamicsSimulated* Simulated)
{
	if (!Simulation) return;
	
	checkf(Simulated, TEXT("Simulated object to unregister cannot be null"));

	if (!RegisteredSimulatedObjects.Contains(Simulated)) return;

	RegisteredSimulatedObjects.Remove(Simulated);
	SimulatedObjectsToRemove.Add(Simulated);

	if (RegisteredSimulatedObjects.Num() == 0)
	{
		if (SimulatedObjectsToRemove.Num() > 0)
		{
			for (auto simulated : SimulatedObjectsToRemove)
				simulated->Release();

			SimulatedObjectsToRemove.Reset();
		}
		
		TickFunction.UnRegisterTickFunction();
		bTickRegistered = false;

		CurWorld = nullptr;

		Simulation->Release();
		Simulation = nullptr;
		
		if (RunOnWorkerThread)
		{
			SimulationThreadWorker->SetWorldRef(nullptr);
			SimulationThreadWorker->EnsureCompletion();
			SimulationThreadWorker.Reset();
		}

		if (CollisionInterface)
		{
			CollisionInterface->Simulation.Reset();
			delete CollisionInterface;
		}
		CollisionInterface = nullptr;

		UE_LOG(LogVICODynamics, All, TEXT("VICODynamics Simulation Released!"));

		IVICODynamicsPlugin::Get().Released = true;
	}
}

void UVDSimulation::AddExplicitColliderMapping(UBaseVDComponent* VDComponent, UPrimitiveComponent* Collider, bool IgnoreConvexShapes)
{
	if (!ExplicitRigidBodyCollisionEnabled || !VDComponent || !Collider || VDComponent->IsPendingKill() || Collider->IsPendingKill() || VDComponent == Collider) return;

#if VICODYNAMICS_USE_PARALLEL
	FScopeLock lock(&ColliderMappingCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

	VDComponent->ExplicitColliders.AddUnique(FExplicitColliderInfo(Collider, IgnoreConvexShapes));

	UE_LOG(LogVICODynamics, All, TEXT("%s | %s - ExplicitCollider Added with %d Shapes"), *VDComponent->GetOwner()->GetName(), *VDComponent->GetName(), VDComponent->ExplicitColliders.Last().PxShapes.Num());
}

void UVDSimulation::RemoveExplicitColliderMapping(UBaseVDComponent* VDComponent, UPrimitiveComponent* Collider)
{
	if (!ExplicitRigidBodyCollisionEnabled || !VDComponent || !Collider) return;

#if VICODYNAMICS_USE_PARALLEL
	FScopeLock lock(&ColliderMappingCritSec);
#endif // VICODYNAMICS_USE_PARALLEL
	
	VDComponent->ExplicitColliders.Remove(FExplicitColliderInfo(Collider, false, true));
}

void UVDSimulation::RemoveAllExplicitColliderMappings(UBaseVDComponent* VDComponent)
{
	if (!ExplicitRigidBodyCollisionEnabled || !VDComponent) return;

#if VICODYNAMICS_USE_PARALLEL
	FScopeLock lock(&ColliderMappingCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

	VDComponent->ExplicitColliders.Empty();
}

void UVDSimulation::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_VDSimTick);
	if (Simulation)
	{
// 		if (RunOnWorkerThread && !SimulationThreadWorker->WorldRef.IsValid())
// 			SimulationThreadWorker->SetWorldRef(GWorld.GetReference());
// 		if (CollisionInterface && !CollisionInterface->Simulation.IsValid())
// 			CollisionInterface->Simulation.Reset(this);

		if (!RunOnWorkerThread)
		{
			//auto World = GWorld.GetReference(); // SKETCHY!! TODO: Find a better way.
			if (CurWorld)
			{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
				if (CollisionInterface && CollisionInterface->DrawCollisionDebug)
					FlushPersistentDebugLines(CurWorld);
#endif

				CollisionInterface->ResetCache();
				{
					SCOPE_CYCLE_COUNTER(STAT_SimTick);
					Simulation->TickSimulation(DeltaTime);
				}
			}
		}

 		if (!RunOnWorkerThread || (SimulationThreadWorker.IsValid() && SimulationThreadWorker->FrameComplete))
 		{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			auto stats = Simulation->GetSimulationStats();

			if (stats)
			{
				SET_DWORD_STAT(STAT_NumParticles, stats->ParticleCount);
				SET_DWORD_STAT(STAT_NumConstraints, stats->ConstraintCount);
				SET_FLOAT_STAT(STAT_Simulation, stats->TotalSimulationTimeMS);
				SET_FLOAT_STAT(STAT_Velocity, stats->VelocityComputationTimeMS);
				SET_FLOAT_STAT(STAT_Constraints, stats->ConstraintSolverTimeMS);
				SET_FLOAT_STAT(STAT_Collision, stats->CollisionTimeMS);
				SET_FLOAT_STAT(STAT_ParticleCollisionBroad, stats->ParticleCollisionBroadPhaseTimeMS);
				SET_FLOAT_STAT(STAT_ParticleCollisionNarrow, stats->ParticleCollisionNarrowPhaseTimeMS);
				SET_FLOAT_STAT(STAT_RigidBodyCollision, stats->RigidBodyCollisionTimeMS);

    			if (bShowOnScreenStats && GEngine)
    				GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, FString::Printf(TEXT("Sim: %.3f\nVel: %.3f\nCon: %.3f\nPCol: %.3f\nRCol: %.3f"), 
						stats->TotalSimulationTimeMS, 
						stats->VelocityComputationTimeMS, 
						stats->ConstraintSolverTimeMS, 
						stats->ParticleCollisionBroadPhaseTimeMS, 
						stats->RigidBodyCollisionTimeMS));

				if (bReportStatsOnNextTick)
				{					
					UE_LOG(LogVICODynamics, All, TEXT("VICODynamics Stats - Particle count: %d, Constraint count: %d"), stats->ParticleCount, stats->ConstraintCount);
				
					bReportStatsOnNextTick = false;
				}
			}
#endif

			for (int32 i = 0; i < RegisteredSimulatedObjects.Num(); i++)
			{
				SCOPE_CYCLE_COUNTER(STAT_VDSync);
				RegisteredSimulatedObjects[i]->Sync();
			}

			if (SimulatedObjectsToRemove.Num() > 0)
			{
				for (auto simulated : SimulatedObjectsToRemove)
					simulated->Release();

				SimulatedObjectsToRemove.Reset();
			}

			if (RunOnWorkerThread && SimulationThreadWorker.IsValid() && SimulationThreadWorker->FrameComplete)
			{
				SimulationThreadWorker->FrameComplete = false;
			}
		}
	}
}

void UVDSimulation::BeginDestroy()
{
	if (Simulation)
	{
		if (SimulatedObjectsToRemove.Num() > 0)
		{
			for (auto simulated : SimulatedObjectsToRemove)
				simulated->Release();
		}
		
		Simulation->Release();
		Simulation = nullptr;
		RegisteredSimulatedObjects.Empty();
		SimulatedObjectsToRemove.Empty();

		CurWorld = nullptr;
	}

	Super::BeginDestroy();
}

void UVDSimulation::UpdateSimStats()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!Simulation) return;

	bReportStatsOnNextTick = true;
#endif
}

void UVDSimulation::SetIsExplicitRigidBodyCollisionEnabled(bool IsEnabled)
{
	if (IsEnabled == ExplicitRigidBodyCollisionEnabled) return;

	ExplicitRigidBodyCollisionEnabled = IsEnabled;
}

void UVDSimulation::SetSubstepCount(int32 SubstepCount)
{
	if (!Simulation) return;
	if (Settings.SimulationIterations == SubstepCount) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.SimulationIterations = FMath::Clamp(SubstepCount, 1, MAX_int32);
	Simulation->SetSimulationIterations(Settings.SimulationIterations);
}

void UVDSimulation::SetConstraintIterationCount(int32 ConstraintIterationCount)
{
	if (!Simulation) return;
	if (Settings.ConstraintIterations == ConstraintIterationCount) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.ConstraintIterations = FMath::Clamp(ConstraintIterationCount, 1, MAX_int32);
	Simulation->SetConstraintIterations(Settings.ConstraintIterations);
}

void UVDSimulation::SetFramerate(int32 Framerate)
{
	if (!Simulation) return;
	if (Settings.SimulationFramerate == Framerate) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.SimulationFramerate = FMath::Clamp(Framerate, 1, MAX_int32);
	Simulation->SetSimulationTimeStep(1.f / static_cast<float>(Settings.SimulationFramerate));
}

void UVDSimulation::SetSelfCollisionEnabled(bool IsEnabled)
{
	if (!Simulation) return;
	if (Settings.EnableSelfCollision == IsEnabled) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.EnableSelfCollision = IsEnabled;
	Simulation->SetSelfCollisionEnabled(IsEnabled);
}

void UVDSimulation::SetPhysXSceneCollisionEnabled(bool IsEnabled)
{
	if (!Simulation) return;
	if (Settings.EnablePhysXSceneCollision == IsEnabled) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.EnablePhysXSceneCollision = IsEnabled;
	Simulation->SetPhysXSceneCollisionEnabled(IsEnabled);
}

void UVDSimulation::SetPerformExtraCollisionResolutionLoop(bool IsEnabled)
{
	if (!Simulation) return;
	if (Settings.PerformExtraCollisionResolutionLoop == IsEnabled) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.PerformExtraCollisionResolutionLoop = IsEnabled;
	Simulation->SetPerformExtraCollisionResolutionLoop(IsEnabled);
}

void UVDSimulation::SetPerFrameCollisionCacheEnable(bool IsEnabled)
{
	if (Settings.bCachePerFrameResults == IsEnabled) return;
	
	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.bCachePerFrameResults = IsEnabled;
}

void UVDSimulation::SetConstrainSimulationTo2DPlane(bool IsEnabled)
{
	if (!Simulation) return;
	if (Settings.ConstrainSimulationTo2DPlane == IsEnabled) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.ConstrainSimulationTo2DPlane = IsEnabled;
	Simulation->SetConstrainToPlane(IsEnabled);
}

void UVDSimulation::SetConstrainPlaneOrigin(FVector Origin)
{
	if (!Simulation) return;
	if (Settings.PlaneOrigin == Origin) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.PlaneOrigin = Origin;
	Simulation->SetConstrainPlaneOrigin(UVectorToVDVector(Origin));
}

void UVDSimulation::SetConstrainPlaneNormal(FVector Normal)
{
	if (!Simulation) return;
	if (Settings.PlaneNormal == Normal) return;

	FScopeLock Lock(&SimulationSettingsCritSec);
	Settings.PlaneNormal = Normal;
	Simulation->SetConstrainPlaneNormal(UVectorToVDVector(Normal));
}

void UVDSimulation::AddWindSource(UVDWindDirectionalSourceComponent* WindSource)
{
	WindSources.AddUnique(WindSource);
}

void UVDSimulation::RemoveWindSource(UVDWindDirectionalSourceComponent* WindSource)
{
	WindSources.Remove(WindSource);
}

void UVDSimulation::GetDirectionalWindParameters(UBaseVDComponent* VDComponent, FVector& OutDirection, float& OutSpeed, float& OutGust) const
{
	if (WindSources.Num() == 0) // Resort to regular Wind Sources
	{
		auto World = GetWorld();
		if (World)
		{
			float MinGust, MaxGust;
			World->Scene->GetDirectionalWindParameters(OutDirection, OutSpeed, MinGust, MaxGust);
			OutGust = 0.f;
		}
		else
		{
			OutDirection = FVector::ZeroVector;
			OutSpeed = 0.f;
			OutGust = 0.f;
		}
		return;
	}

	FVector VDAvgLocation(ForceInitToZero);
	bool bCalculatedAvgLocation = false;

	FVector AccumulatedDirection(0, 0, 0);
	float AccumulatedSpeed = 0.f;
	float AccumulatedGust = 0.f;
	float TotalWeight = 0.0f;
	for (int32 i = 0; i < WindSources.Num(); i++)
	{
		const auto* CurrentSource = WindSources[i];
		if (VDComponent->AffectedByWindSources.Num() > 0)
		{
			for (int32 j = 0; j < VDComponent->AffectedByWindSources.Num(); j++)
			{
				auto WindSource = Cast<UVDWindDirectionalSourceComponent>(VDComponent->AffectedByWindSources[i].GetComponent(nullptr));
				if (WindSource && CurrentSource != WindSource)
					continue;
			}
		}
		else if (CurrentSource->Radius > 0 && VDComponent->Particles.Num() > 2)
		{
			if (!bCalculatedAvgLocation)
			{
				VDAvgLocation += VDComponent->Particles[0]->GetLocation();
				VDAvgLocation += VDComponent->Particles[VDComponent->Particles.Num() / 2]->GetLocation();
				VDAvgLocation += VDComponent->Particles[VDComponent->Particles.Num() - 1]->GetLocation();

				VDAvgLocation /= 3;
				bCalculatedAvgLocation = true;
			}

			if ((CurrentSource->GetComponentLocation() - VDAvgLocation).SizeSquared() > CurrentSource->RadiusSquared)
				continue;
		}

		const float Weight = CurrentSource->Strength + CurrentSource->Speed + CurrentSource->GetCurrentGust(); // This allows for sudden gusts in different directions
		AccumulatedDirection += CurrentSource->GetForwardVector() * Weight;
		AccumulatedSpeed += CurrentSource->Speed * Weight;
		AccumulatedGust += CurrentSource->GetCurrentGust() * Weight;
		TotalWeight += Weight;
	}

	if (TotalWeight == 0)
		TotalWeight = 1;

	AccumulatedDirection /= TotalWeight;
	AccumulatedSpeed /= TotalWeight;
	AccumulatedGust /= TotalWeight;
	AccumulatedDirection.Normalize();

	OutDirection = AccumulatedDirection;
	OutSpeed = AccumulatedSpeed;
	OutGust = AccumulatedGust;
}

bool UVDSimulation::k2QueryBoxOverlaps(FVector Location, FVector HalfExtents, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore, int32 MaxOverlaps /*= 1000*/)
{
	return QueryBoxOverlaps(Location, HalfExtents, Overlaps, SimObjectsToIgnore, MaxOverlaps);
}

bool UVDSimulation::QueryBoxOverlaps(FVector Location, FVector HalfExtents, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore/* = TArray<UVDSimulatedObject*>()*/, int32 MaxOverlaps /*= 1000*/)
{
	SCOPE_CYCLE_COUNTER(STAT_VDSimQueryOverlap);
	if (!Simulation) return false;

	ensureMsgf(Settings.EnableSelfCollision, TEXT("For Overlap Queries to return any results, SelfCollision must be enabled! See Project Settings->VICODynamics or enable in VDSimulationSettings Actor."));

	vicoDynamics::FQueryBoxShape QueryShape(UVectorToVDVector(HalfExtents));
	TArray<vicoDynamics::FSimulatedParticle*> ResultsArray;
	ResultsArray.AddUninitialized(MaxOverlaps);
	vicoDynamics::FQueryOverlapBuffer ResultsBuffer(ResultsArray.GetData(), MaxOverlaps);
	vicoDynamics::FQueryFilter Filter;
	TArray<vicoDynamics::FSimulatedObject*> IgnoredSimObjects;
	if (SimObjectsToIgnore.Num())
	{
		IgnoredSimObjects.Reserve(SimObjectsToIgnore.Num());
		for (int32 i = 0; i < SimObjectsToIgnore.Num(); i++)
			IgnoredSimObjects.Add(static_cast<vicoDynamics::FSimulatedObject*>(SimObjectsToIgnore[i]->GetUnderlyingSimulatedObject()));

		Filter.SimObjectsToIgnore = IgnoredSimObjects.GetData();
		Filter.NumberOfSimObjectsToIgnore = IgnoredSimObjects.Num();
	}

	const auto bOverlapsFound = Simulation->QueryOverlaps(&QueryShape, UVectorToVDVector(Location), ResultsBuffer, Filter.NumberOfSimObjectsToIgnore ? &Filter : nullptr);
	if (bOverlapsFound)
	{
		Overlaps.Reserve(ResultsBuffer.NumberOfOverlapsRecorded);
		for (uint32 i = 0; i < ResultsBuffer.NumberOfOverlapsRecorded; i++)
		{
			Overlaps.Add(static_cast<UVDSimulatedParticle*>(ResultsBuffer.Overlaps[i]->UserData));
		}
	}

	return bOverlapsFound;
}

bool UVDSimulation::k2QuerySphereOverlaps(FVector Location, float Radius, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore, int32 MaxOverlaps /*= 1000*/)
{
	return QuerySphereOverlaps(Location, Radius, Overlaps, SimObjectsToIgnore, MaxOverlaps);
}

bool UVDSimulation::QuerySphereOverlaps(FVector Location, float Radius, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore/* = TArray<UVDSimulatedObject*>()*/, int32 MaxOverlaps /*= 1000*/)
{
	SCOPE_CYCLE_COUNTER(STAT_VDSimQueryOverlap);
	if (!Simulation) return false;

	ensureMsgf(Settings.EnableSelfCollision, TEXT("For Overlap Queries to return any results, SelfCollision must be enabled! See Project Settings->VICODynamics or enable in VDSimulationSettings Actor."));

	vicoDynamics::FQuerySphereShape QueryShape(Radius);
	TArray<vicoDynamics::FSimulatedParticle*> ResultsArray;
	ResultsArray.AddUninitialized(MaxOverlaps);
	vicoDynamics::FQueryOverlapBuffer ResultsBuffer(ResultsArray.GetData(), MaxOverlaps);
	vicoDynamics::FQueryFilter Filter;
	TArray<vicoDynamics::FSimulatedObject*> IgnoredSimObjects;
	if (SimObjectsToIgnore.Num())
	{
		IgnoredSimObjects.Reserve(SimObjectsToIgnore.Num());
		for (int32 i = 0; i < SimObjectsToIgnore.Num(); i++)
			IgnoredSimObjects.Add(static_cast<vicoDynamics::FSimulatedObject*>(SimObjectsToIgnore[i]->GetUnderlyingSimulatedObject()));

		Filter.SimObjectsToIgnore = IgnoredSimObjects.GetData();
		Filter.NumberOfSimObjectsToIgnore = IgnoredSimObjects.Num();
	}

	const auto bOverlapsFound = Simulation->QueryOverlaps(&QueryShape, UVectorToVDVector(Location), ResultsBuffer, Filter.NumberOfSimObjectsToIgnore ? &Filter : nullptr);
	if (bOverlapsFound)
	{
		Overlaps.Reserve(ResultsBuffer.NumberOfOverlapsRecorded);
		for (uint32 i = 0; i < ResultsBuffer.NumberOfOverlapsRecorded; i++)
		{
			Overlaps.Add(static_cast<UVDSimulatedParticle*>(ResultsBuffer.Overlaps[i]->UserData));
		}
	}

	return bOverlapsFound;
}

void UVDSimulation::OnEndLevelPlay(UBaseVDComponent* Notifier)
{
	if (!bWorkerThreadNotifiedOfEndLevel && RunOnWorkerThread)
	{
		if (SimulationThreadWorker.IsValid())
			SimulationThreadWorker->EnsureCompletion();
		bWorkerThreadNotifiedOfEndLevel = true;
	}
}

///////////////////////////////////////////////////////////////////

FVector VDVectorToUVector(const vicoDynamics::FVector3Data& VDVec)
{
	return FVector(VDVec.X, VDVec.Y, VDVec.Z);
}

vicoDynamics::FVector3Data UVectorToVDVector(const FVector& UVec)
{
	return vicoDynamics::FVector3Data(UVec.X, UVec.Y, UVec.Z);
}