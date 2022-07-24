// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "BaseVDComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "VDSimulation.h"

UBaseVDComponent::UBaseVDComponent(const FObjectInitializer& ObjectInitializer)
{
	bHandleRegistrationWithSimulation = true;
	bRegisteredWithSimulation = false;

	CollisionSettings.CollisionEnabled = ECollisionEnabled::QueryAndPhysics;
	CollisionSettings.CollisionResponse.SetAllChannels(ECR_Block);
	CollisionSettings.ObjectType = ECollisionChannel::ECC_PhysicsBody;
	
	GravityScale = 1.f;
	ParticleRadius = 5.f;
	FrictionCoefficient = 1.f;
	RestitutionCoefficient = .25f;
	AttachToRigids = true;
	AttachToParticles = false;
	AttachToParticlesConstraintStiffness = 1.f;
	EndOffset = 100.f;
	DetectionRadiusInflationFactor = 2.f;
	CreateTensionConstraintsForSimulatingRigids = false;
	ManuallyAttachParticles = false;
	TensionForce = 100.f;
	TensionDampening = 1.f;
	AdjustOtherParticleLocationsBasedOnAttachments = true;
	AffectedByGlobalWind = false;
	WindResistance = 30.f;
	CheckForWindOccluders = false;
	MaxWindOccluderDistance = 1000.f;
	WindOccluderTraceSettings.ObjectType = ECollisionChannel::ECC_PhysicsBody;
	WindOccluderTraceSettings.CollisionEnabled = ECollisionEnabled::QueryOnly;

	MinConstraintDistance = .007f;
	SimulateOnlyWhenRendered = false;
	SimulationToggleThreshold = .1f;
	EnableParticleToParticleCollisions = false;
	ConsiderInnerCollisions = false;
	
	ShowParticles = false;

	bShouldToggleSimulationOnNextSync = false;

	bAutoActivate = true;
}

#if WITH_EDITOR
void UBaseVDComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	auto Property = PropertyChangedEvent.Property;

	if (Property)
	{
		const auto PropName = Property->GetFName();
		if (PropName == GET_MEMBER_NAME_CHECKED(UBaseVDComponent, CollisionSettings))
		{
			if (VDSimObject)
				VDSimObject->SetCollisionSettings(CollisionSettings);
		}
		else if (PropName == GET_MEMBER_NAME_CHECKED(UBaseVDComponent, bFillLocationCache))
		{
			if (SimulationInstance && bFillLocationCache)
			{
				CachedStartLocations.Reset(Particles.Num());

				for (int32 i = 0; i < Particles.Num(); i++)
				{
					CachedStartLocations.Add(FVDCachedParticleStartLocation(GetComponentTransform().InverseTransformPosition(Particles[i]->GetLocation()), Particles[i]->IsAttached));
				}

				bFillLocationCache = false;
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

bool UBaseVDComponent::CanEditChange(const UProperty* InProperty) const
{
	// Editing is not allowed during Simulation
	return SimulationInstance ? InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UBaseVDComponent, bFillLocationCache) : Super::CanEditChange(InProperty);
}
#endif // WITH_EDITOR

void UBaseVDComponent::PostLoad()
{
	Super::PostLoad();

	CollisionSettings.CollisionResponse.UpdateResponseContainerFromArray();
}

void UBaseVDComponent::BeginPlay()
{
	

	Super::BeginPlay();
}

void UBaseVDComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (SimulationInstance && (EndPlayReason == EEndPlayReason::EndPlayInEditor || EndPlayReason == EEndPlayReason::LevelTransition || EndPlayReason == EEndPlayReason::Quit))
		SimulationInstance->OnEndLevelPlay(this);

	Super::EndPlay(EndPlayReason);
}

void UBaseVDComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (bHandleRegistrationWithSimulation && bRegisteredWithSimulation)
		UnregisterWithSimulation();

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UBaseVDComponent::OnRegister()
{
	Super::OnRegister();

	if (bHandleRegistrationWithSimulation && !bRegisteredWithSimulation)
		RegisterWithSimulation();
}

void UBaseVDComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TStructOnScope<FActorComponentInstanceData> UBaseVDComponent::GetComponentInstanceData() const
{
	return MakeStructOnScope<FActorComponentInstanceData, FBaseVDComponentInstanceData>(this);
}

void UBaseVDComponent::AddRadialForce(FVector Origin, float Radius, float Strength, enum ERadialImpulseFalloff Falloff, bool bAccelChange /* = false */)
{
	if (!SimulationInstance) return;

	ForcesToApply.Enqueue(FVDForce(Origin, Radius, Strength, Falloff));
}

void UBaseVDComponent::Sync()
{
	const auto particleCount = Particles.Num();

	if (!ManuallyAttachParticles)
	{
		// if attach requested then generate attachment points for overlapping shapes
		if (AttachToRigids)
		{
			AttachToOverlappingRigidBodiesInternal(particleCount);

			// reset attach flag
			AttachToRigids = false;
		}

		if (AttachToParticles)
		{
			const auto QueryRadius = ParticleRadius * DetectionRadiusInflationFactor;
			TArray<UVDSimulatedObject*> IgnoredSimObjects;
			IgnoredSimObjects.Add(VDSimObject);
			for (int32 i = 0; i < particleCount; i++)
			{
				const auto Particle = Particles[i];
				const auto ParticlePos = Particle->GetLocation();
				TArray<UVDSimulatedParticle*> OverlappedParticles;
				if (SimulationInstance->QuerySphereOverlaps(ParticlePos, QueryRadius, OverlappedParticles, IgnoredSimObjects))
				{
					for (int32 j = 0; j < OverlappedParticles.Num(); j++)
					{
						if (auto Constraint = VDSimObject->CreateSpringConstraint(Particle, OverlappedParticles[j], (OverlappedParticles[j]->GetLocation() - ParticlePos).Size(), AttachToParticlesConstraintStiffness))
						{
							ConstraintsToOtherParticles.Add(Constraint);
						}
					}
				}
			}

			AttachToParticles = false;
		}
	}

	while (!AttachmentsToParticlesPendingProcessing.IsEmpty())
	{
		FVDParticleAttachment Attachment;
		AttachmentsToParticlesPendingProcessing.Dequeue(Attachment);
		CreateParticleConstraintsFromAttachment(Attachment);
	}

	if (SimulateOnlyWhenRendered && GetOwner() && GetOwner()->GetGameTimeSinceCreation() > .2f)
	{
		auto World = GetWorld();
		if (World)
		{
			if ((World->TimeSeconds - LastRenderTime) <= SimulationToggleThreshold)
			{
				if (!bIsSimulating)
					bShouldToggleSimulationOnNextSync = true;
			}
			else
			{
				if (bIsSimulating)
					bShouldToggleSimulationOnNextSync = true;
			}
		}
	}

	for (int32 i = 0; i < particleCount; i++)
	{
		UVDSimulatedParticle* Particle = Particles[i];
		Particle->ProcessAttachment(this);

		if (SimulateOnlyWhenRendered)
		{
			if (bShouldToggleSimulationOnNextSync)
			{
				Particle->ToggleSimulation(!bIsSimulating);
			}
		}
	}

	if (SimulateOnlyWhenRendered && bShouldToggleSimulationOnNextSync)
	{
		bShouldToggleSimulationOnNextSync = false;
		bIsSimulating = !bIsSimulating;

		if (!bIsSimulating)
		{
			bSimObjCachedCollisionEnabled = VDSimObject->GetCollisionEnabled();
			VDSimObject->SetCollisionEnabled(false);
		}
		else
		{
			VDSimObject->SetCollisionEnabled(bSimObjCachedCollisionEnabled);
		}
		UE_LOG(LogVICODynamics, All, TEXT("%s Toggled Simulation. Simulating: %d"), *GetName(), bIsSimulating);
	}

	while (!ForcesToApply.IsEmpty())
	{
		FVDForce Force;
		if (ForcesToApply.Dequeue(Force))
		{
			ApplyForce(Force.Origin, Force.Radius, Force.Strength, Force.Falloff);
		}
	}

	if (UseExplicitRigidBodyCollision && bIsSimulating && ExplicitColliders.Num())
	{
		for (int32 i = 0; i < ExplicitColliders.Num(); i++)
		{
			auto& ColliderInfo = ExplicitColliders[i];

			if (FBoxSphereBounds::BoxesIntersect(ColliderInfo.Collider->Bounds, Bounds))
			{
				ColliderInfo.SkipChecks = false;
				continue;
			}
			else if (FBoxSphereBounds::SpheresIntersect(ColliderInfo.Collider->Bounds, Bounds))
			{
				ColliderInfo.SkipChecks = false;
				continue;
			}
			else
			{
				ColliderInfo.SkipChecks = true;
			}
		}
	}

	if (AffectedByGlobalWind && bIsSimulating && VDSimObject)
	{		
		if (SimulationInstance)
		{
			FVector Direction;
			float Speed;
			float Gust;
			SimulationInstance->GetDirectionalWindParameters(this, Direction, Speed, Gust);
			
			const auto GustSpeed = Speed + Gust;

			VDSimObject->SetAffectiveWindVector(Direction * GustSpeed);
			ApproximateWindDrag(Direction, GustSpeed);
		}
	}

	auto World = GetWorld();
	if (World)
	{
		const auto DeltaTime = World->DeltaTimeSeconds;
		AWorldSettings* const WorldSettings = World->GetWorldSettings();
		if (WorldSettings)
		{
			const auto Dilation = WorldSettings->GetEffectiveTimeDilation();
			for (int32 i = 0; i < TensionConstraints.Num(); i++)
				TensionConstraints[i]->ApplyTensionToAttachment(DeltaTime, Dilation);
		}
	}

	// BP Calls
	OnSync();
	if (OnSimulationSync.IsBound())
		OnSimulationSync.Broadcast();
	if (bNotifyOnVDCollision && OnVDCollisionEvent.IsBound())
	{
		for (const auto& CollisionKVP : PendingCollisionNotifications)
		{
			const auto Collision = CollisionKVP.Value;
			if (Collision.OtherComp.IsValid())
				OnVDCollisionEvent.Broadcast(this, Collision.OtherComp.Get(), Collision.HitLocation, Collision.HitNormal, CollisionKVP.Key);
		}

		PendingCollisionNotifications.Reset();
	}
}

void UBaseVDComponent::AttachToOverlappingRigidBodiesInternal(int32 ParticleCount)
{
	// clear out any previous attachments
	ParticleAttachments.SetNum(0);

	FCollisionObjectQueryParams QueryParams;	
	for (int32 i = 0; i < ECollisionChannel::ECC_MAX; i++)
	{
		const ECollisionChannel Channel = static_cast<ECollisionChannel>(i);
		if (CollisionSettings.CollisionResponse.GetResponse(Channel) > ECR_Ignore)
			QueryParams.AddObjectTypesToQuery(Channel);
	}

	for (int32 i = 0; i < ParticleCount; i++)
	{
		if (bUseLocationCache && CachedStartLocations.Num() == Particles.Num())
		{
			if (!CachedStartLocations[i].bIsAttached && !bApplyCachedStartLocationBeforeAttachToRigids)
				continue;
		}

		const auto Particle = Particles[i];
		auto ParticlePos = Particle->GetLocation();

		// perform an overlap check
		FCollisionShape Shape;
		Shape.SetSphere(ParticleRadius * DetectionRadiusInflationFactor); // Inflate the radius to avoid overlap collision bugs

		// Gather overlapping primitives
		TArray<FOverlapResult> Overlaps;
		const auto bOverlapFound = GetWorld()->OverlapMultiByObjectType(Overlaps, ParticlePos, FQuat::Identity,
			QueryParams, Shape, FCollisionQueryParams(TEXT("VDRope_AttachToRigidsOverlap"), false));

		if (!bOverlapFound) continue;

		UPrimitiveComponent* PrimComp = Overlaps[0].Component.Get(); // Just use the first one

		if (PrimComp)
		{
			FBodyInstance* Body = nullptr;
			FName Bone = NAME_None;
			FTransform LocalToWorld;
			const auto SkeletalMeshComp = Cast<USkeletalMeshComponent>(PrimComp);
			if (SkeletalMeshComp)
			{
				Bone = SkeletalMeshComp->GetBoneName(Overlaps[0].ItemIndex);
				Body = SkeletalMeshComp->GetBodyInstance(Bone);
				LocalToWorld = SkeletalMeshComp->GetSocketTransform(Bone);
			}
			else
			{
				Body = PrimComp->GetBodyInstance();
				LocalToWorld = PrimComp->GetComponentToWorld();
			}

			if (!Body)
				continue;

			if (Particle->IsAttached)
				Particle->Detach();

			const FVector LocalPos = LocalToWorld.InverseTransformPositionNoScale(ParticlePos);
			Particle->AttachToComponent(PrimComp, LocalPos, CreateTensionConstraintsForSimulatingRigids && Body->ShouldInstanceSimulatingPhysics(), Bone);
		}
	}
}

void UBaseVDComponent::AddPendingCollisionNotification(UVDSimulatedParticle* Particle, UPrimitiveComponent* OtherComp, const FVector& HitLocation, const FVector& HitNormal)
{
	if (bNotifyOnVDCollision && !PendingCollisionNotifications.Contains(Particle))
	{
		auto& Collision = PendingCollisionNotifications.Emplace(Particle);
		Collision.OtherComp = OtherComp;
		Collision.HitLocation = HitLocation;
		Collision.HitNormal = HitNormal;
	}
}

void UBaseVDComponent::Release()
{
	if (VDSimObject)
	{
		VDSimObject->ReleaseNow(); // Release the SimulatedObject ASAP.
		VDSimObject = nullptr;
	}

	Particles.Reset();
}

void UBaseVDComponent::RegisterWithSimulation()
{
	if ((!GIsEditor || GIsPlayInEditorWorld) && IVICODynamicsPlugin::IsAvailable())
	{
		auto VDSimulation = IVICODynamicsPlugin::Get().VDSimulation;
		if (VDSimulation)
		{
			auto Simulation = VDSimulation->GetSimulation();
			check(Simulation);
			for (int32 i = 0; i < TensionConstraints.Num(); i++)
				Simulation->AddExternalConstraint(TensionConstraints[i].Get());
						
			SimulationInstance = VDSimulation;
			SimulationInstance->Register(this);
			bRegisteredWithSimulation = true;

			CollisionSettings.CollisionResponse.UpdateResponseContainerFromArray();

			VDSimObject = NewObject<UVDSimulatedObject>(this, TEXT("VDSimObject"));
			check(VDSimObject);

			VDSimObject->SetCollisionSettings(CollisionSettings);
			VDSimObject->SetGravityScale(GravityScale);
			VDSimObject->SetMinimumConstraintSatisfactionDistance(MinConstraintDistance);
			VDSimObject->SetSkipRelativeVelocityCheckDuringCollision(SkipRelativeVelocityCheckDuringRigidBodyCollisions);

			bIsSimulating = true;
		}
	}
}

void UBaseVDComponent::UnregisterWithSimulation()
{
	if (SimulationInstance && !SimulationInstance->IsPendingKillOrUnreachable())
	{
		for (int32 i = 0; i < TensionConstraints.Num(); i++)
			SimulationInstance->GetSimulation()->RemoveExternalConstraint(TensionConstraints[i].Get());

		TensionConstraints.Reset();

		VDSimObject->ReleaseNow();
		VDSimObject = nullptr;

		if (SimulationInstance->GetIsExplicitRigidBodyCollisionEnabled())
			SimulationInstance->RemoveAllExplicitColliderMappings(this);
		
		SimulationInstance->Unregister(this);

		SimulationInstance = nullptr;
		bRegisteredWithSimulation = false;
		bIsSimulating = false;
	}
}

void UBaseVDComponent::ApproximateWindDrag(const FVector& Direction, float Speed)
{
	// Nothing needed here
}

void UBaseVDComponent::CreateTensionConstraintsFromAttachment(FVDParticleAttachment& Attachment)
{
	// Nothing needed here, sub-class should override!
}

void UBaseVDComponent::CreateParticleConstraintsFromAttachment(FVDParticleAttachment& Attachment)
{
	if (Attachment.AttachingToAnotherParticle)
	{
		ensureMsgf(Particles.IsValidIndex(Attachment.ParticleIndex), TEXT("Attachment.ParticleIndex is not valid. Value: %d"), Attachment.ParticleIndex);
		auto Particle = Particles[Attachment.ParticleIndex];
		
		auto VDComp = Attachment.IsAttachmentValid() ? Cast<UBaseVDComponent>(Attachment.ComponentRef.GetComponent(GetOwner())) : this;

		if (VDComp)
		{
			auto SocketStr = Attachment.Socket.ToString();
			if (SocketStr.IsNumeric())
			{
				const auto Index = FCString::Atoi(*SocketStr);
				const auto IsIndexValid = VDComp->Particles.IsValidIndex(Index);
				ensureMsgf(IsIndexValid, TEXT("Particle Index in Attachment.Socket is not valid. Value: %d"), Index);
				if (IsIndexValid)
				{
					auto AttachToParticle = VDComp->Particles[Index];

					auto Constraint = VDSimObject->CreateSpringConstraint(Particle, AttachToParticle, (Particle->GetLocation() - AttachToParticle->GetLocation()).Size(), 1.f);

					// DEBUG
					//UE_LOG(LogVICODynamics, Display, TEXT("Attaching Particle %d to %d"), Attachment.ParticleIndex, Index);

					if (Constraint)
						ConstraintsToOtherParticles.Add(Constraint);
				}
			}
		}
	}
}

void UBaseVDComponent::AttachToOverlappingRigidBodies()
{
	AttachToRigids = true; // Will get retriggered on Sync()
}

#if WITH_EDITOR
void UBaseVDComponent::UpdateCollisionProperty()
{
	CollisionSettings.CollisionResponse.UpdateResponseContainerFromArray();
}
#endif // WITH_EDITOR

const TArray<FVDParticleInfo>* UBaseVDComponent::GetParticleBuildInfo() const
{
	// Nothing to return here
	return nullptr;
}

FVector UBaseVDComponent::GetLastAppliedForceFromTensionConstraint(int32 TensionConstraintIndex/* = 0*/) const
{
	if (!TensionConstraints.IsValidIndex(TensionConstraintIndex)) 
		return FVector::ZeroVector;

	return TensionConstraints[TensionConstraintIndex]->LastAppliedForce;
}

FVector UBaseVDComponent::ResolveAttachmentLocation(const FVDParticleAttachment& Attachment)
{
	if (Attachment.IsRelative)
	{
 		const UPrimitiveComponent* PrimComp = Attachment.IsAttachmentValid() ? Cast<UPrimitiveComponent>(Attachment.ComponentRef.GetComponent(GetOwner())) : this;
		const auto AttachedTo = PrimComp ? PrimComp : this;
		const FVector AttachedPos = SimulationInstance ? Attachment.CalculateAttachmentWorldLocationIgnoreScale(AttachedTo) : Attachment.CalculateAttachmentWorldLocation(AttachedTo);
		return AttachedPos;
	}
	else
	{
		return Attachment.Location;
	}
}

void UBaseVDComponent::ApplyForce(FVector Origin, float Radius, float Strength, enum ERadialImpulseFalloff Falloff)
{
	const auto NumParticles = Particles.Num();

	Radius = FMath::Max(Radius, KINDA_SMALL_NUMBER);

	for (int32 i = 0; i < NumParticles; i++)
	{
		auto particle = Particles[i];

		if (particle->GetIsFree())
		{
			const auto Delta = particle->GetLocation() - Origin;
			const auto Distance = Delta.Size();

			if (Distance > Radius) continue;

			const auto ForceNormal = Delta.GetSafeNormal();
			const auto Force = ForceNormal * (Falloff == RIF_Constant ? Strength : ((1.f - (Distance / Radius)) * Strength));
			particle->AddForce(Force);
		}
	}
}

void UBaseVDComponent::SetAffectedByGlobalWind(bool IsAffected)
{
	if (!IsAffected && IsAffected != AffectedByGlobalWind && VDSimObject) // Reset when going to false
		VDSimObject->SetAffectiveWindVector(FVector::ZeroVector);

	AffectedByGlobalWind = IsAffected;
}

void UBaseVDComponent::SetUseExplicitRigidBodyCollision(bool UseExplicitCollision)
{
	UseExplicitRigidBodyCollision = UseExplicitCollision;
}

/////////////////////////////////////////////////////////////////////////////////////////

FBaseVDComponentInstanceData::FBaseVDComponentInstanceData(const UBaseVDComponent* SourceComponent) : FPrimitiveComponentInstanceData(SourceComponent)
{

}

void FBaseVDComponentInstanceData::ApplyToComponent(UActorComponent* Component, const ECacheApplyPhase CacheApplyPhase)
{
	FPrimitiveComponentInstanceData::ApplyToComponent(Component, CacheApplyPhase);

	UBaseVDComponent* NewComponent = CastChecked<UBaseVDComponent>(Component);

#if WITH_EDITOR
	// This is needed to restore transient collision profile data.
	NewComponent->UpdateCollisionProperty();
#endif // #if WITH_EDITOR

	if (SavedProperties.Num() > 0 && Component->IsRegistered())
	{
		Component->MarkRenderStateDirty();
	}
}

bool FBaseVDComponentInstanceData::ContainsData() const
{
	return (SavedProperties.Num() > 0 || AttachedInstanceComponents.Num() > 0);
}

/////////////////////////////////////////////////////////////////////////////////////////

FVDTensionConstraint::FVDTensionConstraint() : Attachment(nullptr), RestDistance(0.f), TensionForce(1.f), ShouldSnapToProxyForRender(false), LastAppliedForce(ForceInitToZero)
{

}

FVDTensionConstraint::~FVDTensionConstraint()
{
	OwnerComp.Reset();
	Attachment = nullptr;
	ProxyParticle.ParticleIndex = -1;
}

void FVDTensionConstraint::Init(UBaseVDComponent* Component, int32 ParticleIndex, FVDParticleAttachment* InAttachment, float InRestDistance, float InTensionForce, float InDampening)
{
	OwnerComp = FWeakObjectPtr(Component);
	ProxyParticle.ParticleIndex = ParticleIndex;
	Attachment = InAttachment;
	RestDistance = InRestDistance;

	checkf(InTensionForce != 0.f, TEXT("TensionForce cannot be 0!"));
	TensionForce = InTensionForce;
	Dampening = FMath::Clamp(InDampening, 0.1f, 1.f);

	ResetProxyParticle();
	CorrSet = false;
}

void FVDTensionConstraint::Solve(float DeltaTime)
{
	if (OwnerComp.IsValid() && Attachment && !CorrSet)
	{
		auto OwnerVDComp = Cast<UBaseVDComponent>(OwnerComp.Get());
		auto Owner = OwnerVDComp->GetOwner();		
		auto Comp = Cast<UPrimitiveComponent>(Attachment->ComponentRef.GetComponent(Owner));
		if (Comp && OwnerVDComp->Particles.Num() && OwnerVDComp->Particles.IsValidIndex(ProxyParticle.ParticleIndex))
		{
			auto Particle = OwnerVDComp->Particles[ProxyParticle.ParticleIndex];
			if (!Particle->GetIsFree()) return;

			const auto CompIsSimulatingPhysics = Comp->IsSimulatingPhysics(Attachment->Socket);
			
			const auto PInvMass = 1.f / TensionForce;
			const auto MInvMass = ProxyParticle.InvMass;

			const auto PLoc = Particle->GetProjectedLocation();
			const auto MLoc = ProxyParticle.ProjectedPosition;

			const auto Delta = PLoc - MLoc;
			const auto dist = Delta.Size();
			if (dist == 0.f) return;

			const auto correction = (((dist - RestDistance) / dist) * Delta);

			const auto PCor = correction * (PInvMass / (PInvMass + MInvMass));
			const auto MCor = correction * (MInvMass / (PInvMass + MInvMass));
			const auto ProjLoc = PLoc - PCor;
			checkf(!PCor.ContainsNaN() && !ProjLoc.ContainsNaN(), TEXT("PLoc: %s | MLoc: %s | PCor: %s | MCor: %s | Correction: %s | Delta: %s"), *PLoc.ToString(), *MLoc.ToString(), *PCor.ToString(), *MCor.ToString(), *correction.ToString(), *Delta.ToString());

			Particle->SetProjectedLocation(ProjLoc);

			if (CompIsSimulatingPhysics)
			{
				ProxyParticle.ProjectedPosition += MCor;
				CorrSet = true;
			}
		}
	}
}

void FVDTensionConstraint::ApplyTensionToAttachment(float DeltaTime, float Dilation)
{
	if (OwnerComp.IsValid() && Attachment && CorrSet)
	{
		LastAppliedForce = FVector::ZeroVector;
		auto OwnerVDComp = Cast<UBaseVDComponent>(OwnerComp.Get());
		auto Comp = Cast<UPrimitiveComponent>(Attachment->ComponentRef.GetComponent(OwnerVDComp->GetOwner()));
		if (Comp && OwnerVDComp->Particles.Num())
		{
			const auto MLoc = Attachment->Socket != NAME_None ? Comp->GetSocketLocation(Attachment->Socket) : Comp->GetComponentToWorld().TransformPositionNoScale(Attachment->Location);

 			const auto SimDT = (1.f / static_cast<float>(OwnerVDComp->SimulationInstance->GetSettings().SimulationFramerate)) * Dilation;
 			const auto CompMass = Comp->GetMass();

			const auto Force = CalculateSpringForceForTimeStep(CompMass, SimDT, MLoc, DeltaTime, Comp, OwnerVDComp);

			if (Comp->IsSimulatingPhysics(Attachment->Socket))
				Comp->AddImpulseAtLocation(Force * SimDT, MLoc, Attachment->Socket);
			LastAppliedForce = Force;
	
			// DEBUG
//  			DrawDebugPoint(OwnerVDComp->GetWorld(), MLoc, 15.f, FColor::Red, false, DeltaTime * 2.f);
//  			DrawDebugPoint(OwnerVDComp->GetWorld(), ProxyParticle.ProjectedPosition, 15.f, FColor::Green, false, DeltaTime * 2.f);
//  			DrawDebugLine(OwnerVDComp->GetWorld(), MLoc, MLoc + Force * SimDT, FColor::Blue, false, DeltaTime * 2.f);
		}

		ResetProxyParticle();
		CorrSet = false;
	}
}

FVector FVDTensionConstraint::CalculateSpringForceForTimeStep(float CompMass, float SimDT, FVector MLoc, float DeltaTime, UPrimitiveComponent* Comp, UBaseVDComponent* OwnerVDComp, FVector PrevForce)
{
	auto Particle = OwnerVDComp->Particles[ProxyParticle.ParticleIndex];

	static const auto SPRING_COEF = .05f;
	//static const auto SPRING_COEF = 8000.f;
	static const auto SPRING_COEF_SQRT = FMath::Sqrt(SPRING_COEF);
	static const auto DAMPENING_COEF = .1f;
	//static const auto DAMPENING_COEF = 140.f;
	static const auto DAMPENING_COEF2 = DAMPENING_COEF * 2.f;

	const auto MRed = (CompMass * TensionForce) / (CompMass + TensionForce);

	const auto k = (MRed / (SimDT * SimDT)) * SPRING_COEF;
	const auto Delta = MLoc - ProxyParticle.ProjectedPosition;
	const auto Normal = Delta.GetSafeNormal();
	const auto kx = k * Delta;
	const auto d = (MRed / DeltaTime) * SPRING_COEF_SQRT * DAMPENING_COEF * Dampening;
	const auto dv = d * (Comp->GetPhysicsLinearVelocityAtPoint(MLoc, Attachment->Socket) - Particle->GetVelocity());//Normal * ((Comp->GetPhysicsLinearVelocityAtPoint(MLoc, Attachment->Socket) - (OwnerVDComp->Particles[ProxyParticle.ParticleIndex]->GetVelocity()) | Normal));
// 	if (Particle->GetIsFree())
// 		Particle->AddForce(((kx + dv) / TensionForce) * (SimDT * SimDT));
	return -kx - dv;
}

void FVDTensionConstraint::ResetProxyParticle()
{
	if (OwnerComp.IsValid() && Attachment)
	{
		auto OwnerVDComp = Cast<UBaseVDComponent>(OwnerComp.Get());
		auto Comp = Cast<UPrimitiveComponent>(Attachment->ComponentRef.GetComponent(OwnerVDComp->GetOwner()));
		if (Comp)
		{
			const auto CompIsSimulatingPhysics = Comp->IsSimulatingPhysics(Attachment->Socket);
			const auto CompMass = CompIsSimulatingPhysics ? Comp->GetMass() : -1.f;
			const auto MInvMass = CompIsSimulatingPhysics ? 1.f / CompMass : 0.f;
			ProxyParticle.InvMass = MInvMass;
			ProxyParticle.Position = ProxyParticle.ProjectedPosition = (Attachment->Socket != NAME_None ? Comp->GetSocketLocation(Attachment->Socket) : 
																										  Comp->GetComponentToWorld().TransformPositionNoScale(Attachment->Location));
																										  //Comp->GetComponentLocation()) + Comp->GetComponentQuat().RotateVector(Attachment->Location * Comp->GetComponentScale());
		}
	}
}
