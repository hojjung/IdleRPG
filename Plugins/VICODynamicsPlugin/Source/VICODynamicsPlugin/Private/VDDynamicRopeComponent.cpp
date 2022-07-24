// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDDynamicRopeComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"

UVDDynamicRopeComponent::UVDDynamicRopeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bHandleRegistrationWithSimulation = false;
	AttachToRigids = false;
	ManuallyAttachParticles = true;
	ParticleAttachments.Reset();
}

void UVDDynamicRopeComponent::OnRegister()
{
	Super::OnRegister();

	UBaseVDComponent::RegisterWithSimulation(); // Skip the UVDRopeComponednts version since we don't want to build just yet, just registering with the Simulation to receive Sync() calls
}

void UVDDynamicRopeComponent::OnUnregister()
{
	Super::OnUnregister();
}

void UVDDynamicRopeComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	UnregisterWithSimulation();
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVDDynamicRopeComponent::Sync()
{
	if (bBuildRopeOnNextSync)
	{
		SetVisibility(false);
		PerformActualRebuild();
		SetVisibility(true);
	}

	Super::Sync();
}

void UVDDynamicRopeComponent::Release()
{
	Super::Release();
}

void UVDDynamicRopeComponent::RebuildRope()
{
	bBuildRopeOnNextSync = true;
}

void UVDDynamicRopeComponent::PerformActualRebuild()
{
 	if (DesiredLength == 0.f) return; // Make sure we actually have a rope to create
 	if (DesiredSegmentCount == 0) return; // We need at least one segment to build a rope

	NumSegments = DesiredSegmentCount; // This will be copied to the SceneProxy to build the vertex/index buffers
	Length = DesiredLength;
	
	UE_LOG(LogVICODynamics, Display, TEXT("Rebuilding Rope %s with %d Segments and %.2f Length"), *GetName(), NumSegments, Length);

	// We don't want to release our SimObject but we do want to release the particles/constraints it owns.
	if (Particles.Num() > 0)
	{
		Particles.Reset();
		ParticleBuildInfo.Reset();

		VDSimObject->RemoveFromSimulation();
	}
	
	RegisterWithSimulation();
}

void UVDDynamicRopeComponent::RebuildRopeAtPoints(float NumSegmentsPerUnrealUnit, const FVector& StartLocation, const FVector& EndLocation, bool bRelativeLocations)
{
	if (ParticleAttachments.Num() != 0)
		ParticleAttachments.Reset();

	DesiredLength = (EndLocation - StartLocation).Size();
	DesiredSegmentCount = FMath::RoundToInt(DesiredLength / NumSegmentsPerUnrealUnit);

	if (bRelativeLocations)
	{
		FVDParticleAttachment StartAttachment;
		StartAttachment.ParticleIndex = 0;
		StartAttachment.IsRelative = true;
		StartAttachment.Location = StartLocation;
		ParticleAttachments.Add(StartAttachment);
		
		FVDParticleAttachment EndAttachment;
		EndAttachment.ParticleIndex = DesiredSegmentCount;
		EndAttachment.IsRelative = true;
		EndAttachment.Location = EndLocation;
		ParticleAttachments.Add(EndAttachment);
	}
	else
	{
		const auto& compTransform = GetComponentTransform();
		
		FVDParticleAttachment StartAttachment;
		StartAttachment.ParticleIndex = 0;
		StartAttachment.IsRelative = true;
		StartAttachment.Location = compTransform.InverseTransformPosition(StartLocation);
		ParticleAttachments.Add(StartAttachment);

		FVDParticleAttachment EndAttachment;
		EndAttachment.ParticleIndex = DesiredSegmentCount;
		EndAttachment.IsRelative = true;
		EndAttachment.Location = compTransform.InverseTransformPosition(EndLocation);
		ParticleAttachments.Add(EndAttachment);
	}

	RebuildRope();
}

void UVDDynamicRopeComponent::RebuildRopeAttached(float NumSegmentsPerUnrealUnit, USceneComponent* StartAttachedTo, FName StartSocket, bool StartSimulateTensionForces, USceneComponent* EndAttachedTo, FName EndSocket, bool EndSimulateTensionForces)
{
	if (ParticleAttachments.Num() != 0)
		ParticleAttachments.Reset();

	FVDParticleAttachment StartAttachment;
	StartAttachment.ParticleIndex = 0;
	StartAttachment.IsRelative = true;
	StartAttachment.ComponentRef.OverrideComponent = StartAttachedTo;
	StartAttachment.Socket = StartSocket;
	StartAttachment.SimulateTensionForces = StartSimulateTensionForces;
	ParticleAttachments.Add(StartAttachment);
	
	DesiredLength = (EndAttachedTo->GetComponentLocation() - StartAttachedTo->GetComponentLocation()).Size();
	DesiredSegmentCount = FMath::RoundToInt(DesiredLength / NumSegmentsPerUnrealUnit);

	FVDParticleAttachment EndAttachment;
	EndAttachment.ParticleIndex = DesiredSegmentCount;
	EndAttachment.IsRelative = true;
	EndAttachment.ComponentRef.OverrideComponent = EndAttachedTo;
	EndAttachment.Socket = EndSocket;
	EndAttachment.SimulateTensionForces = EndSimulateTensionForces;
	ParticleAttachments.Add(EndAttachment);

	RebuildRope();
}
