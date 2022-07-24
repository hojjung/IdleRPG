// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDProceduralClothComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"
#include "Simulation.h"
#include "VDClothRendering.h"

DECLARE_CYCLE_STAT(TEXT("ProcUpdateProxy"), STAT_ProcClothUpdateProxy, STATGROUP_VICODynamicsRendering);

UVDProceduralClothComponent::UVDProceduralClothComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;

	Width = 100.f;
	Height = 100.f;
	NumParticlesWide = 10;
	NumParticlesHigh = 10;

	NumParticles = NumParticlesWide * NumParticlesHigh;

	AttachToRigids = true;
	StretchStiffness = .75f;
	BendStiffness = 0.5f;
	Mass = 10.f;
	DragCoefficient = .47f; // Coefficient for a sphere
	TileMaterialAlongWidth = 1.f;
	TileMaterialAlongHeight = 1.f;

	WindResistance = 50.f;

	TearingEnabled = false;

	bBuildClothOnNextSync = true;
}

void UVDProceduralClothComponent::UpdateSceneProxy(FVDProcduralClothSceneProxy* Proxy)
{
	SCOPE_CYCLE_COUNTER(STAT_ProcClothUpdateProxy);
	if ((Particles.Num() == 0 && ParticleBuildInfo.Num() == 0) || (Particles.Num() < NumParticles && ParticleBuildInfo.Num() < NumParticles)) return;

	// Allocate VDCloth dynamic data
	FVDClothDynamicData* DynamicData = new FVDClothDynamicData;

	// Transform current positions from particles into component-space array
	DynamicData->VDClothPoints.AddUninitialized(NumParticles);
	for (int32 PointIdx = 0; PointIdx < NumParticles; PointIdx++)
	{
		FVector location;
		if (Particles.Num() == 0)
		{
			if (bUseLocationCache && CachedStartLocations.Num() == ParticleBuildInfo.Num())
				location = GetComponentTransform().TransformPosition(CachedStartLocations[PointIdx].Location);
			else
				location = ParticleBuildInfo[PointIdx].Location;
		}
		else
		{
			location = Particles[PointIdx]->GetLocation();
		}

		DynamicData->VDClothPoints[PointIdx] = GetComponentTransform().InverseTransformPosition(location);
	}

	// Snap the visual mesh vertex to the corresponding Proxy Particle location if needed
	for (int32 i = 0; i < TensionConstraints.Num(); i++)
	{
		const auto Constraint = TensionConstraints[i];
		if (Constraint->ShouldSnapToProxyForRender && Particles.IsValidIndex(Constraint->ProxyParticle.ParticleIndex))
		{
			// TODO: Decide if we should snap to the proxy or do a lerp or ???

			if (bSnapToSimulatingAttachment)
				DynamicData->VDClothPoints[Constraint->ProxyParticle.ParticleIndex] = GetComponentTransform().InverseTransformPosition(Constraint->ProxyParticle.ProjectedPosition);
			else
				DynamicData->VDClothPoints[Constraint->ProxyParticle.ParticleIndex] = FMath::Lerp(GetComponentTransform().InverseTransformPosition(Constraint->ProxyParticle.ProjectedPosition), DynamicData->VDClothPoints[Constraint->ProxyParticle.ParticleIndex], .5f);
		}
	}

#if WITH_EDITORONLY_DATA
	for (int32 i = 0; i < ParticleAttachments.Num(); i++)
	{
		if (ParticleAttachments[i].Highlight && ParticleAttachments[i].ParticleIndex < NumParticles)
		{
			const auto location = Particles.Num() == 0 ? ParticleBuildInfo[ParticleAttachments[i].ParticleIndex].Location : Particles[ParticleAttachments[i].ParticleIndex]->GetLocation();
			DynamicData->PointsToHighlight.Add(location);
		}
	}
#endif // WITH_EDITORONLY_DATA

	// Enqueue command to send to render thread
	ENQUEUE_RENDER_COMMAND(FSendVDClothDynamicData)([Proxy, DynamicData](FRHICommandListImmediate& RHICmdList)
	{
		Proxy->SetDynamicData_RenderThread(DynamicData);
	});
}

FPrimitiveSceneProxy* UVDProceduralClothComponent::CreateSceneProxy()
{
	FVDProcduralClothSceneProxy* Proxy = new FVDProcduralClothSceneProxy(this);
	UpdateSceneProxy(Proxy);

	return Proxy;
}

int32 UVDProceduralClothComponent::GetNumMaterials() const
{
	return 1;
}

void UVDProceduralClothComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SimulationInstance)
	{
		if (AttachToRigids)
			Sync(); // Attach to rigid bodies before the first Simulation Tick to avoid any movement due to collision
		else
			CreateClothGeometry(ParticleBuildInfo); // Needed for particle attachments
	}
}

void UVDProceduralClothComponent::OnRegister()
{
	MarkRenderStateDirty();

	Super::OnRegister();

	NumParticles = NumParticlesWide * NumParticlesHigh;
}

void UVDProceduralClothComponent::RegisterWithSimulation()
{
	Super::RegisterWithSimulation();

	if ((!GIsEditor || GIsPlayInEditorWorld) && IVICODynamicsPlugin::IsAvailable())
	{
		if (SimulationInstance)
		{
			Particles.Empty();
			StretchConstraints.Empty();
			BendConstraints.Empty();
			bBuildClothOnNextSync = true;
		}
	}
}

void UVDProceduralClothComponent::BuildSimulatedCloth()
{
	check(VDSimObject);

	Particles.Reserve(NumParticles);
	StretchConstraints.Empty();
	BendConstraints.Empty();

	const float WidthRestDistance = Width / NumParticlesWide;
	const float HeightRestDistance = Height / NumParticlesHigh;

	for (int32 y = 0; y < NumParticlesHigh; y++)
	{
		for (int32 x = 0; x < NumParticlesWide; x++)
		{
			const auto index = x + y * NumParticlesWide;
			auto& particleInfo = ParticleBuildInfo[index];

			UVDSimulatedParticle* Particle = VDSimObject->CreateParticle(particleInfo);
			if (!Particle)
			{
				Particles.Empty();
				return;
			}

			if (ConsiderInnerCollisions)
				Particle->GetUnderlyingParticle()->SetFlags(static_cast<vicoDynamics::EParticleFlag::Type>(vicoDynamics::EParticleFlag::Collision_IncludeNeighbors | vicoDynamics::EParticleFlag::Collision_IgnoreConstrainedTo));

			if (!particleInfo.IsRigidBodyCollisionEnabled)
				Particle->GetUnderlyingParticle()->SetFlags(vicoDynamics::EParticleFlag::Collision_DisableRigidBodyCollision);
			if (!particleInfo.IsParticleCollisionEnabled)
				Particle->GetUnderlyingParticle()->SetFlags(vicoDynamics::EParticleFlag::Collision_DisableParticleCollision);

			Particle->SetFrictionCoefficient(FrictionCoefficient);
			Particle->SetRestitutionCoefficient(RestitutionCoefficient);

			Particles.Add(Particle);

			// create springs between particles
			if (StretchStiffness > 0.0f)
			{
				if (x > 0)
				{
					const auto& P0 = Particles[index];
					const auto& P1 = Particles[index - 1];

					UVDParticleSpringConstraint* Constraint = VDSimObject->CreateSpringConstraint(P0, P1, WidthRestDistance, StretchStiffness);
					if (!Constraint)
					{
						Particles.Empty();
						StretchConstraints.Empty();
						return;
					}
					StretchConstraints.Add(Constraint);
				}

				if (y > 0)
				{
					const auto prevYarticlesIdxAlongY = x + (y - 1) * NumParticlesWide;

					const auto& P0 = Particles[index];
					const auto& P1 = Particles[prevYarticlesIdxAlongY];

					UVDParticleSpringConstraint* Constraint = VDSimObject->CreateSpringConstraint(P0, P1, HeightRestDistance, StretchStiffness);
					if (!Constraint)
					{
						Particles.Empty();
						StretchConstraints.Empty();
						return;
					}
					StretchConstraints.Add(Constraint);
				}
			}

			// create bending springs (connect over three particles)
			if (BendStiffness > 0.0f)
			{
				if (x > 1)
				{
					const auto& P0 = Particles[index];
					const auto& P1 = Particles[index - 2];

					UVDParticleSpringConstraint* Constraint = VDSimObject->CreateSpringConstraint(P0, P1, WidthRestDistance * 2.f, BendStiffness);
					if (!Constraint)
					{
						Particles.Empty();
						BendConstraints.Empty();
						return;
					}
					BendConstraints.Add(Constraint);
				}

				if (y > 1)
				{
					const auto prevYarticlesIdxAlongY = x + (y - 2) * NumParticlesWide;

					const auto& P0 = Particles[index];
					const auto& P1 = Particles[prevYarticlesIdxAlongY];

					UVDParticleSpringConstraint* Constraint = VDSimObject->CreateSpringConstraint(P0, P1, HeightRestDistance * 2, BendStiffness);
					if (!Constraint)
					{
						Particles.Empty();
						BendConstraints.Empty();
						return;
					}
					BendConstraints.Add(Constraint);
				}
			}

			//TODO: Add ability to create diagonal cross tension constraints
		}
	}

	if (ManuallyAttachParticles)
	{
		// Apply user defined attachments
		for (int32 i = 0; i < ParticleAttachments.Num(); i++)
		{
			auto& Attachment = ParticleAttachments[i];

			if (Attachment.ParticleIndex < 0 || Attachment.ParticleIndex >= Particles.Num())
				continue;

			if (Attachment.ComponentRef.ComponentProperty == NAME_None && Attachment.ComponentRef.OtherActor == nullptr && Attachment.ComponentRef.OverrideComponent == nullptr) // Default to being relative to this component.
				Attachment.ComponentRef.OverrideComponent = this;

			auto Particle = Particles[Attachment.ParticleIndex];
			Particle->CopyAttachment(Attachment, false);

			if (Attachment.SimulateTensionForces)
			{
				CreateTensionConstraintsFromAttachment(Attachment);
			}
			else if (Attachment.AttachingToAnotherParticle)
			{
				AttachmentsToParticlesPendingProcessing.Enqueue(Attachment);
			}
		}
	}

	if (bUseLocationCache && CachedStartLocations.Num() == Particles.Num())
	{
		for (int32 i = 0; i < CachedStartLocations.Num(); i++)
		{
			Particles[i]->SetLocation(GetComponentTransform().TransformPosition(CachedStartLocations[i].Location));
		}
	}

	IVICODynamicsPlugin::Get().VDSimulation->UpdateSimStats();
}

void UVDProceduralClothComponent::UnregisterWithSimulation()
{
	Super::UnregisterWithSimulation();
}

FORCEINLINE int32 GetVertIndex(int32 x, int32 y, int32 Width)
{
	return x + (y * Width);
}

void UVDProceduralClothComponent::ApproximateWindDrag(const FVector& Direction, float Speed)
{
	// Here we approximate the changes in drag due to the angle at which a cloth triangle presents itself in reference to the Winds direction
	const auto AbsDirection = Direction.GetAbs();
	const auto MaxDrag = DragCoefficient * WindResistance;

	for (int32 y = 0; y < NumParticlesHigh - 1; y++)
	{
		for (int32 x = 0; x < NumParticlesWide - 1; x++)
		{
			const auto index = GetVertIndex(x, y, NumParticlesWide);

			if (!Particles[index]->GetIsFree()) continue;

			// We only check the current Particle since subsequent iterations will check the rest
			if (CheckForWindOccluders)
			{
				if (IsOccludedFromWind(Particles[index]->GetLocation(), Direction))
				{
					Particles[index]->SetDragCoefficient(DragCoefficient);
					Particles[index]->SetIsWindOccluded(true);
					continue;
				}
				else
				{
					Particles[index]->SetIsWindOccluded(false);
				}
			}

			int32 TL = index;
			int32 BL = GetVertIndex(x, y + 1, NumParticlesWide);
			int32 TR = GetVertIndex(x + 1, y, NumParticlesWide);
			int32 BR = GetVertIndex(x + 1, y + 1, NumParticlesWide);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if (!Particles.IsValidIndex(TL) || !Particles.IsValidIndex(BL) || !Particles.IsValidIndex(TR) || !Particles.IsValidIndex(BR))
				return;
#endif

			//Normals
			auto TriNormal1 = -((Particles[BL]->GetLocation() - Particles[TL]->GetLocation()) ^ (Particles[TR]->GetLocation() - Particles[TL]->GetLocation()));
			auto TriNormal2 = -((Particles[BL]->GetLocation() - Particles[TR]->GetLocation()) ^ (Particles[BR]->GetLocation() - Particles[TR]->GetLocation()));

			TriNormal1.Normalize();
			TriNormal2.Normalize();

			TriNormal1 = TriNormal1.GetAbs();
			TriNormal2 = TriNormal2.GetAbs();

			const auto T1Angle = 1.f - CalculateIncidentAngleToWindVector(AbsDirection, TriNormal1);
			const auto T1CorrectedDrag = DragCoefficient + (MaxDrag * T1Angle);
			const auto T2Angle = 1.f - CalculateIncidentAngleToWindVector(AbsDirection, TriNormal2);
			const auto T2CorrectedDrag = DragCoefficient + (MaxDrag * T2Angle);

			Particles[TR]->SetDragCoefficient(T1CorrectedDrag);
			Particles[BL]->SetDragCoefficient(T1CorrectedDrag);
			Particles[TL]->SetDragCoefficient(T1CorrectedDrag);
			Particles[BR]->SetDragCoefficient(T2CorrectedDrag);
		}
	}
}

void UVDProceduralClothComponent::CreateTensionConstraintsFromAttachment(FVDParticleAttachment& Attachment)
{
	if (Attachment.SimulateTensionForces)
	{
		check(Particles.IsValidIndex(Attachment.ParticleIndex));

		auto PrimComp = Attachment.IsAttachmentValid() ? Cast<UPrimitiveComponent>(Attachment.ComponentRef.GetComponent(GetOwner())) : this;
		if (PrimComp)
		{
			const auto AttachLocation = ResolveAttachmentLocation(Attachment);//Attachment.CalculateAttachmentWorldLocation(PrimComp);
			const auto ParticleLocation = Particles[Attachment.ParticleIndex]->GetLocation();

			// Stretch
			auto TensionConstraintStretch = new FVDTensionConstraint;
			TensionConstraintStretch->Init(this, Attachment.ParticleIndex, &Attachment, (AttachLocation - ParticleLocation).Size(), TensionForce, TensionDampening);
			TensionConstraintStretch->ShouldSnapToProxyForRender = true;
			TensionConstraints.Add(TSharedPtr<FVDTensionConstraint>(TensionConstraintStretch));
			if (bRegisteredWithSimulation)
				SimulationInstance->GetSimulation()->AddExternalConstraint(TensionConstraints.Last().Get());

			// TODO: Decide if and how we want to do the bend constraints...
		}
	}
}

void UVDProceduralClothComponent::CreateClothGeometry(TArray<FVDParticleInfo>& ClothPoints)
{
	// create rope geometry
	ClothPoints.Reset();

	const float WidthRestDistance = Width / NumParticlesWide;
	const float HeightRestDistance = Height / NumParticlesHigh;
	NumParticles = NumParticlesHigh * NumParticlesWide;
	const float PerParticleMass = Mass / NumParticles;

	for (int32 y = 0; y < NumParticlesHigh; y++)
	{
		for (int32 x = 0; x < NumParticlesWide; x++)
		{
			FVector InitialPosition = GetComponentTransform().TransformPosition(FVector(WidthRestDistance * x, 0.f, -HeightRestDistance * y));

			FVDParticleInfo particleInfo;
			particleInfo.IsFree = true;
			particleInfo.Radius = ParticleRadius;
			particleInfo.Location = InitialPosition;
			particleInfo.Mass = PerParticleMass;
			particleInfo.DragCoefficient = DragCoefficient;
			particleInfo.IsParticleCollisionEnabled = EnableParticleToParticleCollisions;
			ClothPoints.Add(particleInfo);
		}
	}

	if (ManuallyAttachParticles)
	{
		// Apply user defined attachments to visualize the attachment in editor and to avoid abrupt snapping on the first frame
		for (const auto& Attachment : ParticleAttachments)
		{
			if (Attachment.ParticleIndex < 0 || Attachment.ParticleIndex >= NumParticles) continue;

			auto& particleInfo = ClothPoints[Attachment.ParticleIndex];
			const auto prevLocation = particleInfo.Location;
			particleInfo.Location = ResolveAttachmentLocation(Attachment);

			if (AdjustOtherParticleLocationsBasedOnAttachments)
			{
				const auto Delta = particleInfo.Location - prevLocation;

				// Offset the other particles to avoid jagged edges and better represent the attachments.
				for (int32 i = Attachment.ParticleIndex; i < ClothPoints.Num(); i++)
				{
					ClothPoints[i].Location += Delta;
				}
			}
		}
	}
}

void UVDProceduralClothComponent::Sync()
{
	if (SimulationInstance)
	{
		if (bBuildClothOnNextSync)
		{
			// create initial geometry
			CreateClothGeometry(ParticleBuildInfo);
			BuildSimulatedCloth();
			bBuildClothOnNextSync = false;
			if (OnSimulationDataCreated.IsBound())
				OnSimulationDataCreated.Broadcast();
		}

		// 		// Still dodgey! Needs polish and possibly a rework to include splitting constraints by adding particles...
		// 		if (ParticleRadius > 1.f && TearingEnabled)
		// 		{
		// 			for (int32 i = Constraints.Num() - 1; i >= 0; i--)
		// 			{
		// 				auto Constraint = Constraints[i];
		// 				if (!Constraint || !Constraint->IsValidLowLevelFast()) continue;
		// 
		// 				auto Particle1 = Constraint->GetParticle1();
		// 				auto Particle2 = Constraint->GetParticle2();
		// 				
		// 				const auto CurrentDistance = (Particle1->GetLocation() - Particle2->GetLocation()).Size();
		// 				if (CurrentDistance > Constraint->GetRestDistance() * 2.f)
		// 				{
		// 					const auto P1LocalIndex = Particles.IndexOfByKey(Particle1);
		// 					const auto P2LocalIndex = Particles.IndexOfByKey(Particle2);
		// 
		//  					if (Tears.ContainsByPredicate([=](const TTuple<int32, int32>& tuple) { return (tuple.Get<0>() == P1LocalIndex && tuple.Get<1>() == P2LocalIndex) || (tuple.Get<0>() == P2LocalIndex && tuple.Get<1>() == P1LocalIndex); }))
		//  						continue;
		// 
		// 					VDSimObject->DestroyConstraint(Constraint);
		// 					Constraints.RemoveAtSwap(i);
		// 					Tears.Add(TTuple<int32, int32>(P1LocalIndex, P2LocalIndex));
		// 					MarkRenderStateDirty();
		// 
		// 					UE_LOG(LogVICODynamics, Display, TEXT("Tore %d, %d"), P1LocalIndex, P2LocalIndex);
		// 				}
		// 			}
		// 		}
	}

	Super::Sync();
}

void UVDProceduralClothComponent::Release()
{
	if (VDSimObject)
	{
		VDSimObject->ReleaseNow(); // Release the SimulatedObject ASAP.
		VDSimObject = nullptr;
	}
}

void UVDProceduralClothComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if we're not actively being simulated then just update the cloth geometry each 
	// frame, this ensures the editor view is updated when modifying parameters
	if (!SimulationInstance)
		CreateClothGeometry(ParticleBuildInfo);

	// Need to send new data to render thread
	MarkRenderDynamicDataDirty();

	// Call this because bounds have changed
	UpdateComponentToWorld();
}

void UVDProceduralClothComponent::SendRenderDynamicData_Concurrent()
{
	if (SceneProxy)
	{
		UpdateSceneProxy((FVDProcduralClothSceneProxy*)SceneProxy);
	}
}

FBoxSphereBounds UVDProceduralClothComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	// Calculate bounding box of Cloth particles
	FBox ClothBox(ForceInitToZero);
	auto Count = SimulationInstance ? Particles.Num() : ParticleBuildInfo.Num();

	for (int32 ParticleIdx = 0; ParticleIdx < Count; ParticleIdx++)
	{
		const auto& Position = SimulationInstance ? Particles[ParticleIdx]->GetLocation() : ParticleBuildInfo[ParticleIdx].Location;
		ClothBox += Position;
	}

	// Expand by rope width
	return FBoxSphereBounds(ClothBox.ExpandBy(ParticleRadius));
}