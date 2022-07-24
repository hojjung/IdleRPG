// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDSkinnedRopeComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"

UVDSkinnedRopeComponent::UVDSkinnedRopeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

FPrimitiveSceneProxy* UVDSkinnedRopeComponent::CreateSceneProxy()
{
	return nullptr; // Nothing to render
}

void UVDSkinnedRopeComponent::RegisterWithSimulation()
{
	auto SkinnedMesh = Cast<UPoseableMeshComponent>(PoseableMeshRef.GetComponent(GetOwner()));

	if (SkinnedMesh && Cast<UVDSkinnedRopeComponent>(SkinnedMesh->GetAttachParent()))
		SkinnedMesh->bUseAttachParentBound = true; // Make sure the mesh is using the dynamic bounds if parented to us. Otherwise we see shadowing and rendering artifacts.
	
	Super::RegisterWithSimulation();
}

void UVDSkinnedRopeComponent::Sync()
{
	Super::Sync();

	auto SkinnedMesh = Cast<UPoseableMeshComponent>(PoseableMeshRef.GetComponent(GetOwner()));

	if (SkinnedMesh && Particles.Num() == Bones.Num())
	{
		FVector BasisX = (Particles[1]->GetLocation() - Particles[0]->GetLocation()).GetSafeNormal();
		FVector BasisY, BasisZ;
		BasisX.FindBestAxisVectors(BasisY, BasisZ);

		for (int32 i = 1; i < Bones.Num(); i++)
		{
			auto P1 = Particles[i - 1];
			auto P2 = Particles[i];
			const auto P1Loc = P1->GetLocation();
			const auto P2Loc = P2->GetLocation();

			const auto Normal = (P2Loc - P1Loc).GetSafeNormal();
						
			SkinnedMesh->SetBoneLocationByName(Bones[i - 1], P1Loc, EBoneSpaces::WorldSpace);
			SkinnedMesh->SetBoneLocationByName(Bones[i], P2Loc, EBoneSpaces::WorldSpace);
					
			FVector RotationAxis = (BasisX ^ Normal).GetSafeNormal();
			float CosTheta = Normal | BasisX;

			// Use a Frenet frame to create a smooth basis along the rope
			if (FMath::Abs(CosTheta - 1.0f) > KINDA_SMALL_NUMBER)
			{
				BasisX = Normal;

				float Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
				//BasisY = BasisY.RotateAngleAxis(Theta, RotationAxis); // Not used
				BasisZ = BasisZ.RotateAngleAxis(Theta, RotationAxis);
			}

			SkinnedMesh->SetBoneRotationByName(Bones[i - 1], FRotationMatrix::MakeFromXZ(BasisX, BasisZ).Rotator(), EBoneSpaces::WorldSpace);

			if (i == Bones.Num() - 1)
				SkinnedMesh->SetBoneRotationByName(Bones[i], FRotationMatrix::MakeFromXZ(BasisX, BasisZ).Rotator(), EBoneSpaces::WorldSpace);
		}
	}
}

float CalucalteTotalLengthFromBones(const TArray<FName>& Bones, UPoseableMeshComponent* SkinnedMesh)
{
	float TotalLength = 0.f;

	for (int32 i = 1; i < Bones.Num(); i++)
	{
		const auto Bone1Location = SkinnedMesh->GetBoneLocationByName(Bones[i - 1], EBoneSpaces::WorldSpace);
		const auto Bone2Location = SkinnedMesh->GetBoneLocationByName(Bones[i], EBoneSpaces::WorldSpace);

		TotalLength += (Bone2Location - Bone1Location).Size();
	}

	return TotalLength;
}

void UVDSkinnedRopeComponent::CreateRopeGeometry(TArray<FVDParticleInfo>& RopePoints)
{	
	auto SkinnedMesh = Cast<UPoseableMeshComponent>(PoseableMeshRef.GetComponent(GetOwner()));

	if (SkinnedMesh && SkinnedMesh->SkeletalMesh)
	{
		SkinnedMesh->GetBoneNames(Bones);

		// Filter out root bone
		for (int32 i = 0; i < Bones.Num(); i++)
		{
			if (Bones[i].ToString().Contains(TEXT("Root")))
			{
				Bones.RemoveAt(i);
				i--;
				// If we only filter the root, we can break here
				break;
			}
		}

		Bones.Sort();

		NumSegments = Bones.Num() - 1;

		Length = CalucalteTotalLengthFromBones(Bones, SkinnedMesh);
		if (Length > 0.f)
		{
			RopePoints.Reset();

			const int32 NumParticles = NumSegments + 1;
			const float PerParticleMass = Mass / NumParticles;

			for (int32 i = 0; i < NumParticles; i++)
			{
				const FVector InitialPosition = SkinnedMesh->GetBoneLocationByName(Bones[i], EBoneSpaces::WorldSpace);

				FVDParticleInfo particleInfo;
				particleInfo.IsFree = true;
				particleInfo.Radius = ParticleRadius;
				particleInfo.Location = InitialPosition;
				particleInfo.Mass = PerParticleMass;
				particleInfo.DragCoefficient = DragCoefficient;
				RopePoints.Add(particleInfo);
			}

			if (ManuallyAttachParticles)
			{
				// Apply user defined attachments, to visualize the attachment in editor
				for (auto& Attachment : ParticleAttachments)
				{
					if (Attachment.ParticleIndex < 0 || Attachment.ParticleIndex >= NumParticles) continue;

					auto& particleInfo = RopePoints[Attachment.ParticleIndex];
					const auto prevLocation = particleInfo.Location;
					particleInfo.Location = ResolveAttachmentLocation(Attachment);
					particleInfo.IsFree = false;
					Attachment.OldMass = particleInfo.Mass;
					particleInfo.Mass = 0.f;
				}
			}
		}
	}
}

void UVDSkinnedRopeComponent::BuildSimulatedRope()
{
	CreateRopeGeometry(ParticleBuildInfo);
	Super::BuildSimulatedRope();
}
