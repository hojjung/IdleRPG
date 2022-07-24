// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDMeshClothComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"
#include "Simulation.h"
#include "VDClothRendering.h"
#include "StaticMeshResources.h"

UVDMeshClothComponent::UVDMeshClothComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;

	StretchStiffness = .75f;
	BendStiffness = 0.5f;
	Mass = 10.f;
	DragCoefficient = .47f; // Coefficient for a sphere

	WindResistance = 50.f;

	TearingEnabled = false;

	UseVertexColorDataForSetup = false;

	MaxBendConstraintDistance = 100.f;
}

void UVDMeshClothComponent::UpdateSceneProxy(FVDMeshClothSceneProxy* Proxy)
{
	if (!HullMesh) return;
	if ((Particles.Num() == 0 && ParticleBuildInfo.Num() == 0) || (Particles.Num() < NumParticles && ParticleBuildInfo.Num() < NumParticles)) return;

	// Allocate VDCloth dynamic data
	FVDClothDynamicData* DynamicData = new FVDClothDynamicData;

	// Transform current positions from particles into component-space array
	DynamicData->VDClothPoints.Reserve(NumParticles);
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

		DynamicData->VDClothPoints.Add(GetComponentTransform().InverseTransformPosition(location));

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
	ENQUEUE_RENDER_COMMAND(FSendVDMeshClothDynamicData)([Proxy, DynamicData](FRHICommandListImmediate& RHICmdList)
	{
		Proxy->SetDynamicData_RenderThread(DynamicData);
	});
}

FPrimitiveSceneProxy* UVDMeshClothComponent::CreateSceneProxy()
{
	if (HullMesh && SourceIndices.Num() >= 3 && NumParticles == SimulatedVertices.Num())
	{
		FVDMeshClothSceneProxy* Proxy = new FVDMeshClothSceneProxy(this);
		UpdateSceneProxy(Proxy);

		return Proxy;
	}

	return nullptr;
}

int32 UVDMeshClothComponent::GetNumMaterials() const
{
	return 1;//HullMesh ? HullMesh->Materials.Num() : 1;
}

void UVDMeshClothComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SimulationInstance && HullMesh)
	{
		if (AttachToRigids)
			Sync(); // Attach to rigid bodies before the first Simulation Tick to avoid any movement due to collision
		else
			CreateClothGeometry(ParticleBuildInfo); // Needed for particle attachments		
	}
}

void UVDMeshClothComponent::OnRegister()
{
	MarkRenderStateDirty();

	Super::OnRegister();
}

void UVDMeshClothComponent::RegisterWithSimulation()
{
	Super::RegisterWithSimulation();

	if (HullMesh)
	{
		// create initial geometry
		CreateClothGeometry(ParticleBuildInfo);

		if ((!GIsEditor || GIsPlayInEditorWorld) && IVICODynamicsPlugin::IsAvailable())
		{
			if (SimulationInstance)
			{
				bBuildOnNextSync = true;
			}
		}
	}
}

void UVDMeshClothComponent::BuildSimulatedCloth()
{
	check(VDSimObject);

	Particles.Reserve(NumParticles);
	StretchConstraints.Empty();
	BendConstraints.Empty();

	for (int32 i = 0; i < NumParticles; i++)
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
	}

	for (int32 i = 0; i < SimulatedIndices.Num(); i += 3)
	{
		const auto Idx1 = SimulatedIndices[i];
		const auto Idx2 = SimulatedIndices[i + 1];
		const auto Idx3 = SimulatedIndices[i + 2];

		UVDSimulatedParticle* P1 = Particles[Idx1];
		UVDSimulatedParticle* P2 = Particles[Idx2];
		UVDSimulatedParticle* P3 = Particles[Idx3];

		StretchConstraints.Add(VDSimObject->CreateSpringConstraint(P1, P2, (P1->GetLocation() - P2->GetLocation()).Size(), StretchStiffness));
		StretchConstraints.Add(VDSimObject->CreateSpringConstraint(P1, P3, (P1->GetLocation() - P3->GetLocation()).Size(), StretchStiffness));
		StretchConstraints.Add(VDSimObject->CreateSpringConstraint(P2, P3, (P2->GetLocation() - P3->GetLocation()).Size(), StretchStiffness));

		// Bend constraints
		if (i >= 3 && BendStiffness > 0.f/* && i % 6 == 0*/)
		{
			const auto PrevIdx1 = SimulatedIndices[i - 1];
			const auto PrevIdx2 = SimulatedIndices[i - 2];
			const auto PrevIdx3 = SimulatedIndices[i - 3];

			UVDSimulatedParticle* PrevP1 = Particles[PrevIdx1];
			UVDSimulatedParticle* PrevP2 = Particles[PrevIdx2];
			UVDSimulatedParticle* PrevP3 = Particles[PrevIdx3];

			if (PrevP1->GetUnderlyingParticle() != P1->GetUnderlyingParticle())
			{
				const auto P1PrevP1Dist = (P1->GetLocation() - PrevP1->GetLocation()).Size();
				if (P1PrevP1Dist <= MaxBendConstraintDistance)
					BendConstraints.Add(VDSimObject->CreateSpringConstraint(P1, PrevP1, P1PrevP1Dist, BendStiffness));
			}
			if (PrevP2->GetUnderlyingParticle() != P2->GetUnderlyingParticle())
			{
				const auto P2PrevP2Dist = (P2->GetLocation() - PrevP2->GetLocation()).Size();
				if (P2PrevP2Dist <= MaxBendConstraintDistance)
					BendConstraints.Add(VDSimObject->CreateSpringConstraint(P2, PrevP2, P2PrevP2Dist, BendStiffness));
			}
			if (PrevP3->GetUnderlyingParticle() != P3->GetUnderlyingParticle())
			{
				const auto P3PrevP3Dist = (P3->GetLocation() - PrevP3->GetLocation()).Size();
				if (P3PrevP3Dist <= MaxBendConstraintDistance)
					BendConstraints.Add(VDSimObject->CreateSpringConstraint(P3, PrevP3, P3PrevP3Dist, BendStiffness));
			}
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

void UVDMeshClothComponent::UnregisterWithSimulation()
{
	Super::UnregisterWithSimulation();
}

void UVDMeshClothComponent::ApproximateWindDrag(const FVector& Direction, float Speed)
{
	// Here we approximate the changes in drag due to the angle at which a cloth triangle presents itself in reference to the Winds direction
	const auto AbsDirection = Direction.GetAbs();
	const auto MaxDrag = DragCoefficient * WindResistance;

	for (int32 i = 0; i < SimulatedIndices.Num(); i += 3)
	{
		auto P1Idx = SimulatedIndices[i];
		auto P2Idx = SimulatedIndices[i + 1];
		auto P3Idx = SimulatedIndices[i + 2];

		auto Particle1 = Particles[P1Idx];
		auto Particle2 = Particles[P2Idx];
		auto Particle3 = Particles[P3Idx];

		// Since we are doing a single trace per triangle; we check to make sure all members are not fixed
		if (!Particle1->GetIsFree() && !Particle2->GetIsFree() && !Particle3->GetIsFree()) continue;

		const auto P1Location = Particle1->GetLocation();
		const auto P2Location = Particle2->GetLocation();
		const auto P3Location = Particle3->GetLocation();

		if (CheckForWindOccluders)
		{
			const auto Centroid = (P1Location + P2Location + P3Location) * .3333333f; // Find the centroid location
			if (IsOccludedFromWind(Centroid, Direction))
			{
				Particle1->SetDragCoefficient(DragCoefficient);
				Particle2->SetDragCoefficient(DragCoefficient);
				Particle3->SetDragCoefficient(DragCoefficient);

				Particle1->SetIsWindOccluded(true);
				Particle2->SetIsWindOccluded(true);
				Particle3->SetIsWindOccluded(true);
				continue;
			}
			else
			{
				Particle1->SetIsWindOccluded(false);
				Particle2->SetIsWindOccluded(false);
				Particle3->SetIsWindOccluded(false);
			}
		}

		//Normal
		auto TriNormal1 = -((P2Location - P1Location) ^ (P3Location - P1Location));

		TriNormal1.Normalize();

		const auto T1Angle = CalculateIncidentAngleToWindVector(AbsDirection, TriNormal1.GetAbs());
		const auto T1CorrectedDrag = DragCoefficient + (MaxDrag * T1Angle);

		Particle1->SetDragCoefficient(T1CorrectedDrag);
		Particle2->SetDragCoefficient(T1CorrectedDrag);
		Particle3->SetDragCoefficient(T1CorrectedDrag);
	}
}

void UVDMeshClothComponent::CreateTensionConstraintsFromAttachment(FVDParticleAttachment& Attachment)
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

const TArray<FVDParticleInfo>* UVDMeshClothComponent::GetParticleBuildInfo() const
{
	return &ParticleBuildInfo;
}

// Removes duplicate vertices in the Vertices array, updates the Indices array and creates a mapping from the duplicates
void RemoveDuplicateVertices(TArray<FStaticMeshBuildVertex>& Vertices, TArray<uint32>& Indices, TArray<int32>& Mappings)
{
	TMap<uint32, uint32> DuplicateVerts; // Track the verts to remove
	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		if (DuplicateVerts.Contains(i)) // Avoid removing the same duplicates multiple times and simply record the mapping
		{
			Mappings[i] = DuplicateVerts[i];
			continue;
		}

		const auto Pos = Vertices[i].Position;
		Mappings[i] = i; // Map the current vertex in case it's unique

		for (int32 j = i + 1; j < Vertices.Num(); j++)
		{
			const auto Pos2 = Vertices[j].Position;

			if ((Pos - Pos2).GetAbs().IsNearlyZero(KINDA_SMALL_NUMBER)) // Check if it is indeed unique
			{
				// It isn't, mark it for removal and record the mapping

				DuplicateVerts.Add(j, i);
				Mappings[j] = i;
			}
		}
	}

	// Sort in a descending order to remove from the end, this way we do not mess up the previously recorded duplicate indices
	DuplicateVerts.KeySort([](uint32 A, uint32 B) {return A > B;});

	for (const auto& kvp : DuplicateVerts)
	{
		Vertices.RemoveAt(kvp.Key);

		// After removal, we must replace its entry with the unique one then fix up the Indices array and shift everything past it over by 1
		for (int32 i = 0; i < Indices.Num(); i++)
		{
			if (Indices[i] == kvp.Key)
				Indices[i] = kvp.Value; // The replacement

			if (Indices[i] > kvp.Key && Indices[i] > 0) // The fix-up and shift of all indices past it
				Indices[i]--;
		}

		// Lastly, for every duplicate we remove, we need to shift the mappings which are past it by 1
		for (int32 i = 0; i < Mappings.Num(); i++)
		{
			if (Mappings[i] > (int32)kvp.Key && Mappings[i] > 0)
				Mappings[i]--;
		}
	}
}

void UVDMeshClothComponent::CreateClothGeometry(TArray<FVDParticleInfo>& ClothPoints)
{
	// create rope geometry
	ClothPoints.Reset();

	if (!HullMesh) return;
	if (!HullMesh->bAllowCPUAccess && SimulationInstance)
		UE_LOG(LogVICODynamics, Warning, TEXT("%s - %s - %s Mesh Asset does not have AllowCPUAccess set to true. This compopnent will fail to build and Simulate in a Cooked build!"), *GetOwner()->GetName(), *GetName(), *HullMesh->GetName());

	SourceIndices.Empty();

	const FStaticMeshLODResources& MeshResources = HullMesh->GetLODForExport(0); // Add LOD Selection as property!

	SourceVertices.Empty();
	SourceVertices.AddZeroed(MeshResources.GetNumVertices());

	// Grab a copy of the vertex data from the HullMesh, needed for rendering and building the cloth
	for (int32 i = 0; i < MeshResources.GetNumVertices(); i++)
	{
		auto& SourceVertex = SourceVertices[i];
		SourceVertex.Position = MeshResources.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
		SourceVertex.Color = MeshResources.VertexBuffers.ColorVertexBuffer.GetNumVertices() > 0 ? MeshResources.VertexBuffers.ColorVertexBuffer.VertexColor(i) : FColor::White;

		for (uint32 uvIdx = 0; uvIdx < MeshResources.VertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords(); uvIdx++)
			SourceVertex.UVs[uvIdx] = MeshResources.VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, uvIdx);
	}

	MeshResources.IndexBuffer.GetCopy(SourceIndices);

	// Create a copy of each for using the Source in the rendering, SceneProxy, and the Simulated to build the cloth
	SimulatedVertices = SourceVertices;
	SimulatedIndices = SourceIndices;
	SourceToSimulatedVertexMappings.Empty();
	SourceToSimulatedVertexMappings.AddUninitialized(SourceVertices.Num());

	// We need to get rid of the duplicate vertices otherwise a cube mesh, for example, would unfold at the seams
	// We also need to store the mappings from Source to Simulated vertex indices for rendering everything correctly
	RemoveDuplicateVertices(SimulatedVertices, SimulatedIndices, SourceToSimulatedVertexMappings);

	NumParticles = SimulatedVertices.Num();
	const float PerParticleMass = Mass / NumParticles;

	for (int32 i = 0; i < NumParticles; i++)
	{
		FVector InitialPosition = GetComponentTransform().TransformPosition(SimulatedVertices[i].Position);

		FVDParticleInfo particleInfo;
		particleInfo.IsFree = true;
		if (UseVertexColorDataForSetup)
		{
			particleInfo.IsRigidBodyCollisionEnabled = SimulatedVertices[i].Color.R > 0;
			particleInfo.IsParticleCollisionEnabled = SimulatedVertices[i].Color.G > 0;
		}
		else
		{
			particleInfo.IsParticleCollisionEnabled = EnableParticleToParticleCollisions;
		}
		particleInfo.Radius = ParticleRadius;
		particleInfo.Location = InitialPosition;
		particleInfo.Mass = PerParticleMass;
		particleInfo.DragCoefficient = DragCoefficient;
		ClothPoints.Add(particleInfo);
	}

	if (ManuallyAttachParticles)
	{
		ParticleAttachments.Sort();

		// Apply user defined attachments to visualize the attachment in editor and to avoid abrupt snapping on the first frame
		for (const auto& Attachment : ParticleAttachments)
		{
			if (Attachment.ParticleIndex < 0 || Attachment.ParticleIndex >= NumParticles) continue;

			if (Attachment.AttachingToAnotherParticle && SimulationInstance)
			{
				ParticleAttachmentsToResolveOnBuild.Add(Attachment);
				continue;
			}

			auto& particleInfo = ClothPoints[Attachment.ParticleIndex];
			const auto prevLocation = particleInfo.Location;
			particleInfo.Location = ResolveAttachmentLocation(Attachment);

			if (AdjustOtherParticleLocationsBasedOnAttachments)
			{
				const auto Delta = particleInfo.Location - prevLocation;

				// Offset the other particles to avoid jagged edges and better represent the attachments.
				for (int32 i = Attachment.ParticleIndex + 1; i < ClothPoints.Num(); i++)
				{
					ClothPoints[i].Location += Delta;
				}
			}
		}
	}
}

void UVDMeshClothComponent::Sync()
{
	if (bBuildOnNextSync)
	{
		ResolvePreBuildParticleAttachments();
		BuildSimulatedCloth();
		bBuildOnNextSync = false;
		if (OnSimulationDataCreated.IsBound())
			OnSimulationDataCreated.Broadcast();
		return;
	}

	// 	if (SimulationInstance)
	// 	{
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
	// 	}

	Super::Sync();
}

void UVDMeshClothComponent::ResolvePreBuildParticleAttachments()
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

void UVDMeshClothComponent::Release()
{
	if (VDSimObject)
	{
		VDSimObject->ReleaseNow(); // Release the SimulatedObject ASAP.
		VDSimObject = nullptr;
	}
}

void UVDMeshClothComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UVDMeshClothComponent::SendRenderDynamicData_Concurrent()
{
	if (SceneProxy)
	{
		UpdateSceneProxy((FVDMeshClothSceneProxy*)SceneProxy);
	}
}

FVector UVDMeshClothComponent::GetSocketLocation(FName InSocketName) const
{
	auto SocketStr = InSocketName.ToString();
	SocketStr.RemoveFromStart(TEXT("Mesh Particle "));
	if (SocketStr.IsNumeric())
	{
		const auto Index = FCString::Atoi(*SocketStr);

		if (Index >= 0 && Index < NumParticles && (Particles.Num() == NumParticles || ParticleBuildInfo.Num() == NumParticles))
		{
			return Particles.Num() ? Particles[Index]->GetLocation() : ParticleBuildInfo[Index].Location;
		}
	}

	return Super::GetSocketLocation(InSocketName);
}

FQuat UVDMeshClothComponent::GetSocketQuaternion(FName InSocketName) const
{
	return Super::GetSocketQuaternion(InSocketName);
}

FRotator UVDMeshClothComponent::GetSocketRotation(FName InSocketName) const
{
	return Super::GetSocketRotation(InSocketName);
}

bool UVDMeshClothComponent::HasAnySockets() const
{
	return true;
}

FTransform UVDMeshClothComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace /*= RTS_World*/) const
{
	auto SocketStr = InSocketName.ToString();
	SocketStr.RemoveFromStart(TEXT("Mesh Particle "));
	if (SocketStr.IsNumeric())
	{
		const auto Index = FCString::Atoi(*SocketStr);

		if (Index >= 0 && Index < NumParticles && (Particles.Num() == NumParticles || ParticleBuildInfo.Num() == NumParticles))
		{
			// World space already
			auto ParticleLocation = Particles.Num() ? Particles[Index]->GetLocation() : ParticleBuildInfo[Index].Location;

			if (TransformSpace == RTS_Component)
				ParticleLocation = GetComponentTransform().InverseTransformPosition(ParticleLocation);
			else if (TransformSpace == RTS_Actor)
				ParticleLocation = GetOwner()->GetActorTransform().InverseTransformPosition(ParticleLocation);

			return FTransform(FRotator::ZeroRotator, ParticleLocation, GetComponentScale());
		}
	}

	return Super::GetSocketTransform(InSocketName);
}

void UVDMeshClothComponent::QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const
{
	const auto Count = SimulationInstance ? Particles.Num() : ParticleBuildInfo.Num();
	for (int32 i = 0; i < Count; i++)
	{
		OutSockets.Add(FComponentSocketDescription(FName(*(FString::Printf(TEXT("Mesh Particle %d"), i))), EComponentSocketType::Socket));
	}
}

FBoxSphereBounds UVDMeshClothComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	// Calculate bounding box of Cloth particles
	FBox ClothBox(ForceInitToZero);
	const auto Count = SimulationInstance ? Particles.Num() : ParticleBuildInfo.Num();

	for (int32 ParticleIdx = 0; ParticleIdx < Count; ParticleIdx++)
	{
		const auto& Position = SimulationInstance ? Particles[ParticleIdx]->GetLocation() : ParticleBuildInfo[ParticleIdx].Location;
		ClothBox += Position;
	}

	// Expand by radius
	auto bounds = FBoxSphereBounds(ClothBox.ExpandBy(ParticleRadius));
	if (bounds.ContainsNaN())
		bounds = FBoxSphereBounds(GetComponentLocation(), FVector(ParticleRadius * NumParticles), ParticleRadius * NumParticles);
	return bounds;
}