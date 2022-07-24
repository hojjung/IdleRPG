// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDRopeComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"
#include "Simulation.h"
#include "VDRopeRendering.h"
#include "Misc/CString.h"

DECLARE_CYCLE_STAT(TEXT("UpdateRopeProxy"), STAT_UpdateRopeProxy, STATGROUP_VICODynamicsRendering);

UVDRopeComponent::UVDRopeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;

	Width = 10.f;
	NumSegments = 10;
	NumSides = 8;
	Length = EndOffset;
	StretchStiffness = .75f;
	BendStiffness = 0.5f;
	ParticleRadius = 5.f;
	Mass = NumSegments + 1;
	DragCoefficient = .47f; // Coefficient for a sphere
	TileMaterial = 1.f;

	WindResistance = 25.f;

	bBuildRopeOnNextSync = false;
}

void UVDRopeComponent::UpdateSceneProxy(FVDRopeSceneProxy* Proxy)
{
	SCOPE_CYCLE_COUNTER(STAT_UpdateRopeProxy);

	int32 NumPoints = NumSegments + 1;

	if ((Particles.Num() == 0 && ParticleBuildInfo.Num() == 0) || (Particles.Num() < NumPoints && ParticleBuildInfo.Num() < NumPoints)) return;


	// Allocate VDRope dynamic data
	FVDRopeDynamicData* DynamicData = new FVDRopeDynamicData;

	// Transform current positions from particles into component-space array
	DynamicData->VDRopePoints.AddUninitialized(NumPoints);
	for (int32 PointIdx = 0; PointIdx < NumPoints; PointIdx++)
	{
//		FVector location;
		if (Particles.Num() > 0)
		{
//			location = Particles[PointIdx]->GetLocation();
			DynamicData->VDRopePoints[PointIdx] = GetComponentTransform().InverseTransformPosition(Particles[PointIdx]->GetLocation());
		}
		else		
		{
			if (bUseLocationCache && CachedStartLocations.Num() == ParticleBuildInfo.Num())
//				location = GetComponentTransform().TransformPosition(CachedStartLocations[PointIdx].Location);
				DynamicData->VDRopePoints[PointIdx] = GetComponentTransform().InverseTransformPosition(GetComponentTransform().TransformPosition(CachedStartLocations[PointIdx].Location));
			else
//				location = ParticleBuildInfo[PointIdx].Location;
				DynamicData->VDRopePoints[PointIdx] = GetComponentTransform().InverseTransformPosition(ParticleBuildInfo[PointIdx].Location);
		}
//		DynamicData->VDRopePoints[PointIdx] = GetComponentTransform().InverseTransformPosition(location);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (ShowParticles)
		{
			FVDDebugRenderData DebugData;
			DebugData.IsFree = Particles.Num() == 0 ? ParticleBuildInfo[PointIdx].IsFree : Particles[PointIdx]->GetIsFree();
			DebugData.Radius = ParticleRadius;
			DynamicData->DebugRenderData.Add(DebugData);
		}
#endif
	}

	// Snap the visual mesh segment to the corresponding Proxy Particle location if needed
	for (int32 i = 0; i < TensionConstraints.Num(); i++)
	{
		const auto Constraint = TensionConstraints[i];
		if (Constraint->ShouldSnapToProxyForRender && Particles.IsValidIndex(Constraint->ProxyParticle.ParticleIndex))
		{
			// TODO: Decide if we should snap to the proxy or do a lerp or ???

			if (bSnapToSimulatingAttachment)
				DynamicData->VDRopePoints[Constraint->ProxyParticle.ParticleIndex] = GetComponentTransform().InverseTransformPosition(Constraint->ProxyParticle.ProjectedPosition);
			else
				DynamicData->VDRopePoints[Constraint->ProxyParticle.ParticleIndex] = FMath::Lerp(GetComponentTransform().InverseTransformPosition(Constraint->ProxyParticle.ProjectedPosition), DynamicData->VDRopePoints[Constraint->ProxyParticle.ParticleIndex], .5f);
		}
	}

	// Enqueue command to send to render thread	
	ENQUEUE_RENDER_COMMAND(FSendVDRopeDynamicData)([Proxy, DynamicData](FRHICommandListImmediate& RHICmdList)
	{
		Proxy->SetDynamicData_RenderThread(DynamicData);
	});
}

FPrimitiveSceneProxy* UVDRopeComponent::CreateSceneProxy()
{
	FVDRopeSceneProxy* Proxy = new FVDRopeSceneProxy(this);
	UpdateSceneProxy(Proxy);

	return Proxy;
}

FVector UVDRopeComponent::GetSocketLocation(FName InSocketName) const
{
	auto SocketStr = InSocketName.ToString();
	SocketStr.RemoveFromStart(TEXT("Rope Particle "));
	if (SocketStr.IsNumeric())
	{
		const auto Index = FCString::Atoi(*SocketStr);

		if (Index >= 0 && Index <= NumSegments && (Particles.Num() == NumSegments + 1 || ParticleBuildInfo.Num() == NumSegments + 1))
		{
			return Particles.Num() ? Particles[Index]->GetLocation() : ((bUseLocationCache && CachedStartLocations.IsValidIndex(Index)) ? 
																		GetComponentTransform().TransformPosition(CachedStartLocations[Index].Location) : ParticleBuildInfo[Index].Location);
		}
	}

	return Super::GetSocketLocation(InSocketName);
}

FQuat UVDRopeComponent::GetSocketQuaternion(FName InSocketName) const
{
	return Super::GetSocketQuaternion(InSocketName);
}

FRotator UVDRopeComponent::GetSocketRotation(FName InSocketName) const
{
	return Super::GetSocketRotation(InSocketName);
}

bool UVDRopeComponent::HasAnySockets() const
{
	return true;
}

FTransform UVDRopeComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace /*= RTS_World*/) const
{
	auto SocketStr = InSocketName.ToString();
	SocketStr.RemoveFromStart(TEXT("Rope Particle "));
	if (SocketStr.IsNumeric())
	{
		const auto Index = FCString::Atoi(*SocketStr);

		if (Index >= 0 && Index <= NumSegments && (Particles.Num() == NumSegments + 1 || ParticleBuildInfo.Num() == NumSegments + 1))
		{
			// World space already
			auto ParticleLocation = Particles.Num() ? Particles[Index]->GetLocation() : ((bUseLocationCache && CachedStartLocations.IsValidIndex(Index)) ? 
																						GetComponentTransform().TransformPosition(CachedStartLocations[Index].Location) : ParticleBuildInfo[Index].Location);

			if (TransformSpace == RTS_Component)
				ParticleLocation = GetComponentTransform().InverseTransformPosition(ParticleLocation);
			else if (TransformSpace == RTS_Actor)
				ParticleLocation = GetOwner()->GetActorTransform().InverseTransformPosition(ParticleLocation);

			return FTransform(FRotator::ZeroRotator, ParticleLocation, GetComponentScale());
		}
		else if (SimulationInstance)
		{
			UE_LOG(LogVICODynamics, Error, TEXT("%s - %s: Could not return a valid Particle Socket Transform. Attachment will default to Components origin!"), *GetOwner()->GetName(), *GetName());
		}
	}

	return Super::GetSocketTransform(InSocketName);
}

void UVDRopeComponent::QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const
{
	for (int32 i = 0; i <= NumSegments; i++)
	{
		OutSockets.Add(FComponentSocketDescription(FName(*(FString::Printf(TEXT("Rope Particle %d"), i))), EComponentSocketType::Socket));
	}
}

int32 UVDRopeComponent::GetNumMaterials() const
{
	return 1;
}

void UVDRopeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SimulationInstance)
	{
		if (AttachToRigids)
			Sync(); // Attach to rigid bodies before the first Simulation Tick to avoid any movement due to collision
		else
			CreateRopeGeometry(ParticleBuildInfo); // Needed for particle attachments
	}
}

void UVDRopeComponent::RegisterWithSimulation()
{
	Super::RegisterWithSimulation();

	if ((!GIsEditor || GIsPlayInEditorWorld) && IVICODynamicsPlugin::IsAvailable())
	{
		if (SimulationInstance)
		{
			Particles.Empty();
			StretchConstraints.Empty();
			BendConstraints.Empty();
			bBuildRopeOnNextSync = true;
		}
	}
}

void UVDRopeComponent::BuildSimulatedRope()
{
	check(VDSimObject);

	const int32 NumParticles = NumSegments + 1;

	Particles.Reserve(NumParticles);

	const float RestDistance = Length / NumSegments;

	for (int32 i = 0; i < ParticleBuildInfo.Num(); i++)
	{
		auto& particleInfo = ParticleBuildInfo[i];
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
		if (i > 0 && StretchStiffness > 0.0f)
		{
			const auto& P0 = Particles[i];
			const auto& P1 = Particles[i - 1];

			UVDParticleSpringConstraint* Constraint = VDSimObject->CreateSpringConstraint(P0, P1, RestDistance, StretchStiffness);
			if (!Constraint)
			{
				Particles.Empty();
				StretchConstraints.Empty();
				BendConstraints.Empty();
				return;
			}
			StretchConstraints.Add(Constraint);
		}

		// create bending springs (connect over three particles)
		if (i > 1 && BendStiffness > 0.0f)
		{
			const auto& P0 = Particles[i];
			const auto& P1 = Particles[i - 2];

			//const auto RDist = P1->GetIsFree() ? RestDistance * 2.f : (P0->GetLocation() - P1->GetLocation()).Size();
			UVDParticleSpringConstraint* Constraint = VDSimObject->CreateSpringConstraint(P0, P1, RestDistance * 2.f, BendStiffness);
			if (!Constraint)
			{
				Particles.Empty();
				StretchConstraints.Empty();
				BendConstraints.Empty();
				return;
			}
			BendConstraints.Add(Constraint);
		}
	}

	if (ManuallyAttachParticles)
	{
		for (int32 i = 0; i < ParticleAttachments.Num(); i++)
		{
			auto& Attachment = ParticleAttachments[i];

			if (Attachment.ParticleIndex < 0 || Attachment.ParticleIndex > NumSegments || Attachment.ParticleIndex >= Particles.Num())
				continue;

			if (Attachment.ComponentRef.ComponentProperty == NAME_None && Attachment.ComponentRef.OtherActor == nullptr && Attachment.ComponentRef.OverrideComponent == nullptr) // Default to being relative to this component.
				Attachment.ComponentRef.OverrideComponent = this;

			auto Particle = Particles[Attachment.ParticleIndex];
			Particle->CopyAttachment(Attachment, false);

			if (Attachment.SimulateTensionForces)
			{
				CreateTensionConstraintsFromAttachment(Particle->Attachment);
			}
			else if (Attachment.AttachingToAnotherParticle)
			{
				AttachmentsToParticlesPendingProcessing.Enqueue(Particle->Attachment);
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

	Pieces.Reset();
	Pieces.Add(FRopePiece(0, NumSegments));
	
	IVICODynamicsPlugin::Get().VDSimulation->UpdateSimStats();
}

void UVDRopeComponent::SetNewRestLength(float NewLength, int32 PieceIndex/* = -1*/)
{
	if (NewLength <= 0.f || PieceIndex < -1 || PieceIndex >= Pieces.Num()) return;

	const auto StretchLength = NewLength / static_cast<float>(NumSegments);
	const auto BendLength = StretchLength * 2.f;

	if (PieceIndex >= 0)
	{
		SetNewLengthsForPiece(Pieces[PieceIndex], StretchLength, BendLength);
	}
	else
	{
		for (int32 i = 0; i < Pieces.Num(); i++)
			SetNewLengthsForPiece(Pieces[i], StretchLength, BendLength);
	}

	Length = NewLength;
}

void UVDRopeComponent::SetNewRestLengthStartingAtParticle(int32 ParticleIndexStart, float NewLength)
{
	if (NewLength <= 0.f || ParticleIndexStart >= Particles.Num()) return;

	const auto StretchLength = NewLength / static_cast<float>(NumSegments);
	const auto BendLength = StretchLength * 2.f;

	FRopePiece TempPiece;
	TempPiece.StartParticleIndex = ParticleIndexStart;
	TempPiece.EndParticleIndex = NumSegments;
	SetNewLengthsForPiece(TempPiece, StretchLength, BendLength);

	// Needed?
	Length = NewLength;
}

void UVDRopeComponent::SetNewLengthsForPiece(const FRopePiece& Piece, const float StretchLength, const float BendLength)
{
	for (int32 i = 0; i < StretchConstraints.Num(); i++)
	{
		auto Constraint = StretchConstraints[i];
		if (Piece.Contains(Particles.IndexOfByKey(Constraint->GetParticle1())) || Piece.Contains(Particles.IndexOfByKey(Constraint->GetParticle2())))
			Constraint->SetRestDistance(StretchLength);
	}

	for (int32 i = 0; i < BendConstraints.Num(); i++)
	{
		auto Constraint = BendConstraints[i];
		if (Piece.Contains(Particles.IndexOfByKey(Constraint->GetParticle1())) || Piece.Contains(Particles.IndexOfByKey(Constraint->GetParticle2())))
			Constraint->SetRestDistance(BendLength);
	}
}

float UVDRopeComponent::GetTrueLength(int32 PieceIndex/* = 0*/) const
{
	float TrueLength = 0.f;

	if (Particles.Num() && Pieces.IsValidIndex(PieceIndex))
	{
		const auto Piece = Pieces[PieceIndex];
		for (int32 i = Piece.StartParticleIndex; i < Piece.EndParticleIndex; i++)
		{
			const auto P1 = Particles[i];
			const auto P2 = Particles[i + 1];

			TrueLength += (P1->GetLocation() - P2->GetLocation()).Size();
		}
	}
	else
	{
		TrueLength = Length;
	}

	return TrueLength;
}

void UVDRopeComponent::BreakAtSegment(int32 SegmentIndex)
{
	if (SegmentIndex >= NumSegments || SegmentIndex < 0 || !SimulationInstance) return;

	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		if (Pieces[i].StartParticleIndex == SegmentIndex)
			return;
	}

	auto Particle1 = Particles[SegmentIndex];
	auto Particle2 = Particles[SegmentIndex + 1];

	TArray<int32> ConstraintsRemoved;
	for (int32 i = 0; i < StretchConstraints.Num(); i++)
	{
		auto Constraint = StretchConstraints[i];
		if (Constraint->IsConstraining(Particle1, Particle2))
		{
			VDSimObject->DestroyConstraint(Constraint);
			ConstraintsRemoved.Add(i);
		}
	}

	ConstraintsRemoved.Sort();
	for (int32 i = ConstraintsRemoved.Num() - 1; i >= 0; i--)
		StretchConstraints.RemoveAtSwap(ConstraintsRemoved[i]);

	ConstraintsRemoved.Reset();

	if (BendStiffness > 0 && Particles.IsValidIndex(SegmentIndex - 1) && Particles.IsValidIndex(SegmentIndex + 2))
	{
		auto ParticlePre1 = Particles[SegmentIndex - 1];
		auto ParticlePost2 = Particles[SegmentIndex + 2];

		for (int32 i = 0; i < BendConstraints.Num(); i++)
		{
			auto Constraint = BendConstraints[i];
			if (Constraint->IsConstraining(ParticlePre1, Particle2) || Constraint->IsConstraining(Particle1, ParticlePost2))
			{
				VDSimObject->DestroyConstraint(Constraint);
				ConstraintsRemoved.Add(i);
			}
		}

		ConstraintsRemoved.Sort();
		for (int32 i = ConstraintsRemoved.Num() - 1; i >= 0; i--)
			BendConstraints.RemoveAtSwap(ConstraintsRemoved[i]);
	}

	for (int32 i = 0; i < Pieces.Num(); i++)
	{
		FRopePiece Piece = Pieces[i];
		if (Piece.Contains(SegmentIndex))
		{
			FRopePiece Piece1, Piece2;
			if (Piece.Split(SegmentIndex, Piece1, Piece2))
			{
				Pieces.RemoveAtSwap(i);
				Pieces.Add(Piece1);
				Pieces.Add(Piece2);
				Pieces.Sort();
			}
			break;
		}
	}

	// Recreate SceneProxy
	SetVisibility(false);
	SetVisibility(true);
}

void UVDRopeComponent::CorrectConstraintsForAttachedParticles()
{
	for (int32 i = 0; i < BendConstraints.Num(); i++)
	{
		auto BendConstraint = BendConstraints[i];
		auto P1 = BendConstraint->GetParticle1();
		auto P2 = BendConstraint->GetParticle2();

		if (!P1->GetIsFree() || !P2->GetIsFree())
		{
			BendConstraint->SetRestDistance((P1->GetLocation() - P2->GetLocation()).Size());

			if (!P1->GetIsFree())
			{
				for (int32 j = 0; j < StretchConstraints.Num(); j++)
				{
					auto StretchConstraint = StretchConstraints[i];
					if (StretchConstraint->GetParticle2() == P2)
						StretchConstraint->SetRestDistance((P2->GetLocation() - StretchConstraint->GetParticle1()->GetLocation()).Size());
				}
			}
		}
	}
}

void UVDRopeComponent::ResolvePreBuildParticleAttachments()
{
	if (ParticleAttachmentsToResolveOnBuild.Num() > 0)
	{
		for (int32 i = 0; i < ParticleAttachmentsToResolveOnBuild.Num(); i++)
		{
			auto Attachment = ParticleAttachmentsToResolveOnBuild[i];

			auto& particleInfo = ParticleBuildInfo[Attachment.ParticleIndex];
			const auto prevLocation = particleInfo.Location;
			particleInfo.Location = ResolveAttachmentLocation(Attachment);

			if (AdjustOtherParticleLocationsBasedOnAttachments)
			{
				const auto Delta = particleInfo.Location - prevLocation;

				// Offset the other particles to avoid jagged edges and better represent the attachments.
				for (int32 j = 0; j < ParticleBuildInfo.Num() && j != Attachment.ParticleIndex; j++)
				{
					ParticleBuildInfo[j].Location += Delta;
				}
			}
		}

		ParticleAttachmentsToResolveOnBuild.Empty();
	}
}

void UVDRopeComponent::UnregisterWithSimulation()
{
	Super::UnregisterWithSimulation();

	StretchConstraints.Empty();
	BendConstraints.Empty();
}

void UVDRopeComponent::GetEndPositions(FVector& OutStartPosition, FVector& OutEndPosition)
{
	FVDParticleAttachment* startAttachment = nullptr;
	FVDParticleAttachment* endAttachment = nullptr;

	if (ManuallyAttachParticles)
	{
		// Find the end point attachments if they exist
		for (auto& Attachment : ParticleAttachments)
		{
			if (Attachment.ParticleIndex == 0)
				startAttachment = &Attachment;
			else if (Attachment.ParticleIndex == NumSegments)
				endAttachment = &Attachment;

			// Don't exit early in case we have duplicates, grab the last added.
			// TODO: Add duplicate avoidance/detection!
		}
	}

	if (startAttachment)
	{
		OutStartPosition = ResolveAttachmentLocation(*startAttachment);
	}
	else
	{
		OutStartPosition = GetComponentLocation();
	}

	if (endAttachment)
	{
		OutEndPosition = ResolveAttachmentLocation(*endAttachment);
	}
	else
	{
		OutEndPosition = OutStartPosition + FVector(AttachToRigids ? EndOffset : Length, 0.f, 0.f);
	}
}

void UVDRopeComponent::CreateRopeGeometry(TArray<FVDParticleInfo>& RopePoints)
{
	// create rope geometry
	RopePoints.Reset();

	FVector RopeStart, RopeEnd;
	GetEndPositions(RopeStart, RopeEnd);

	const int32 NumParticles = NumSegments + 1;

	const FVector Delta = RopeEnd - RopeStart;
	const float PerParticleOffset = Delta.Size() / NumSegments;
	const float PerParticleMass = Mass / NumParticles;

	for (int32 ParticleIdx = 0; ParticleIdx < NumParticles; ParticleIdx++)
	{
		const float Alpha = (float) ParticleIdx / (float) NumSegments;
		const FVector InitialPosition = GetComponentTransform().TransformPositionNoScale(FVector(PerParticleOffset * ParticleIdx, 0.f, 0.f));

		FVDParticleInfo particleInfo;
		particleInfo.IsFree = true;
		particleInfo.Radius = ParticleRadius;
		particleInfo.Location = InitialPosition;
		particleInfo.Mass = PerParticleMass;
		particleInfo.DragCoefficient = DragCoefficient;
		particleInfo.IsParticleCollisionEnabled = EnableParticleToParticleCollisions;
		RopePoints.Add(particleInfo);
	}

	if (ManuallyAttachParticles)
	{
		ParticleAttachments.StableSort();

		// Apply user defined attachments, to visualize the attachment in editor
		for (auto& Attachment : ParticleAttachments)
		{
			//-1 Now means the "end" of the rope, so you don't have to know how long the rope is
			int32 ParticleIndex = Attachment.ParticleIndex;
			if (ParticleIndex < 0)
				ParticleIndex = NumParticles - 1;

			if (ParticleIndex < 0 || ParticleIndex >= NumParticles) continue;

			if (Attachment.AttachingToAnotherParticle && SimulationInstance)
			{
				ParticleAttachmentsToResolveOnBuild.Add(Attachment);
				continue;
			}

			auto& particleInfo = RopePoints[ParticleIndex];
			const auto prevLocation = particleInfo.Location;
			particleInfo.Location = ResolveAttachmentLocation(Attachment);

			if (AdjustOtherParticleLocationsBasedOnAttachments)
			{
				const auto DeltaDueToAttachment = particleInfo.Location - prevLocation;

				// Offset the other particles to avoid jagged edges and better represent the attachments.
				for (int32 i = ParticleIndex + 1; i < NumParticles; i++)
				{
					RopePoints[i].Location += DeltaDueToAttachment;
				}
			}
		}
	}
}

void UVDRopeComponent::Sync()
{
	if (SimulationInstance)
	{
		if (bBuildRopeOnNextSync)
		{
			// create initial geometry
			CreateRopeGeometry(ParticleBuildInfo);
			ResolvePreBuildParticleAttachments();
			BuildSimulatedRope();
			if (AttachToRigids)
			{
				AttachToOverlappingRigidBodiesInternal(Particles.Num());
				AttachToRigids = false; // Reset early here
//				CorrectConstraintsForAttachedParticles();
			}
			bBuildRopeOnNextSync = false;
			if (OnSimulationDataCreated.IsBound())
				OnSimulationDataCreated.Broadcast();
			return;
		}
	
		if (IsVisible())
		{
			// Need to send new data to render thread
			MarkRenderDynamicDataDirty();
			UpdateComponentToWorld();
			// Call this because bounds have changed
			UpdateBounds();
		}
	}

	Super::Sync();
}

void UVDRopeComponent::ApproximateWindDrag(const FVector& Direction, float Speed)
{
	// Here we approximate the changes in drag due to the angle at which a rope segment presents itself in reference to the Winds direction
	const auto AbsWindDirection = Direction.GetAbs();
	const auto MaxDrag = DragCoefficient * WindResistance;

	for (int32 i = 1; i < Particles.Num(); i++)
	{
		auto Particle1 = Particles[i];

		if (!Particle1->GetIsFree()) continue;

		auto Particle2 = Particles[i - 1];

		if (CheckForWindOccluders)
		{
			if (IsOccludedFromWind(Particle1->GetLocation(), Direction))
			{
				// Reset drag when occluded
				Particle1->SetDragCoefficient(DragCoefficient);
				Particle2->SetDragCoefficient(DragCoefficient);

				Particle1->SetIsWindOccluded(true);
				Particle2->SetIsWindOccluded(true);
				continue;
			}
			else
			{
				Particle1->SetIsWindOccluded(false);
				Particle2->SetIsWindOccluded(false);
			}
		}

		auto Delta = Particle1->GetLocation() - Particle2->GetLocation();
		auto AbsSegNormal = Delta.GetSafeNormal().GetAbs();

		const auto Angle = CalculateIncidentAngleToWindVector(AbsWindDirection, AbsSegNormal);
		const auto CorrectedDrag = DragCoefficient + (MaxDrag * Angle);
		Particle1->SetDragCoefficient(CorrectedDrag);
		Particle2->SetDragCoefficient(CorrectedDrag);

// 		if (i == Particles.Num() - 1)
// 		{
// 			GEngine->AddOnScreenDebugMessage(INDEX_NONE, GetWorld()->DeltaTimeSeconds, FColor::Green, FString::Printf(TEXT("WindNorm = %s - SegNorm = %s - Angle = %.3f"), *AbsWindDirection.ToCompactString(), *AbsSegNormal.ToCompactString(), Angle));
// 		}
	}
}

void UVDRopeComponent::CreateTensionConstraintsFromAttachment(FVDParticleAttachment& Attachment)
{
	if (Attachment.SimulateTensionForces)
	{
		check(Particles.IsValidIndex(Attachment.ParticleIndex));

		const int32 NumParticles = NumSegments + 1;

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

// 			if (NumParticles > 1 && BendStiffness > 0.0f && Particles[Attachment.ParticleIndex]->GetIsFree()) // TODO: Refine bend constraint creation: add option, add smarter selection to avoid picking a particle already attached to us
// 			{
// 				const auto BendParticleIndex = Attachment.ParticleIndex < NumParticles - 1 ? Attachment.ParticleIndex + 1 : Attachment.ParticleIndex - 1;
// 				const auto BendParticleLocation = Particles[BendParticleIndex]->GetLocation();
// 
// 				// Bend
// 				auto TensionConstraintBend = new FVDTensionConstraint;
// 				TensionConstraintBend->Init(this, BendParticleIndex, &Attachment, (AttachLocation - BendParticleLocation).Size(), TensionForce, TensionDampening);
// 				TensionConstraints.Add(TSharedPtr<FVDTensionConstraint>(TensionConstraintBend));
// 				if (bRegisteredWithSimulation)
// 					SimulationInstance->GetSimulation()->AddExternalConstraint(TensionConstraints.Last().Get());
// 			}
		}
	}
}

const TArray<FVDParticleInfo>* UVDRopeComponent::GetParticleBuildInfo() const
{
	return &ParticleBuildInfo;
}

void UVDRopeComponent::Release()
{
	Super::Release();
}

void UVDRopeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// if we're not actively being simulated then just update the rope geometry each 
	// frame, this ensures the editor view is updated when modifying parameters
	if (!SimulationInstance)
	{
		CreateRopeGeometry(ParticleBuildInfo);

		// Need to send new data to render thread
		MarkRenderDynamicDataDirty();
		// Call this because bounds have changed
		UpdateComponentToWorld();
	}
}

void UVDRopeComponent::SendRenderDynamicData_Concurrent()
{
	if (SceneProxy)
	{
		UpdateSceneProxy((FVDRopeSceneProxy*) SceneProxy);
	}
}

void UVDRopeComponent::CreateRenderState_Concurrent()
{
	Super::CreateRenderState_Concurrent();

	SendRenderDynamicData_Concurrent();
}

FBoxSphereBounds UVDRopeComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	// Calculate bounding box of Rope particles
	FBox RopeBox(ForceInitToZero);
	auto Count = SimulationInstance ? Particles.Num() : ParticleBuildInfo.Num();

	for (int32 ParticleIdx = 0; ParticleIdx < Count; ParticleIdx++)
	{
		auto Position = SimulationInstance ? Particles[ParticleIdx]->GetLocation() : ParticleBuildInfo[ParticleIdx].Location;
		if (Position.ContainsNaN())
			continue;
		
		RopeBox += Position;
	}

	// Expand by rope width
	return FBoxSphereBounds(RopeBox.ExpandBy(Width));
}