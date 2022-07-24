// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Components/MeshComponent.h"

#include "VDRopeComponent.h"
#include "VDSimulation.h"
#include "VDSimulatedParticle.h"

#include "VDDynamicRopeComponent.generated.h"

/**
 * 
 */
UCLASS(hidecategories = (Attachment), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class VICODYNAMICSPLUGIN_API UVDDynamicRopeComponent : public UVDRopeComponent
{
public:
	GENERATED_BODY()

	UVDDynamicRopeComponent(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Begin UActorComponent interface.
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	// End UActorComponent interface.
	
public:
	// Begin IVICODynamicsSimulated Interface
	/* Signals that a simulate tick has finished and it is safe to access/modify Particle and/or Constraint data */
	virtual void Sync() override;
	
	/* Perform any Simulation related clean up */
	virtual void Release() override;
	// End IVICODynamicsSimulated Interface
	
	/**
	 * Schedules a new rope to be created on the next Sync
	 */
	UFUNCTION(BlueprintCallable, Category = "VDynamics|Rope")
	virtual void RebuildRope();

	/**
	 * Schedules a new rope to be created on the next Sync with the end points at the specified locations
	 * @param DistancePerSegment - The distance, in Unreal Units, between segments. Determines segment count
	 */
	UFUNCTION(BlueprintCallable, Category = "VDynamics|Rope")
	void RebuildRopeAtPoints(float DistancePerSegment, const FVector& StartLocation, const FVector& EndLocation, bool bRelativeLocations);

	/**
	 * Schedules a new rope to be created on the next Sync attached to the specified Actors/Components, optionally at the specified sockets.
	 * Note: You can still edit the *LocationOffset properties to offset the attachment point.
	 * @param DistancePerSegment - The distance, in Unreal Units, between segments. Determines segment count
	 */
	UFUNCTION(BlueprintCallable, Category = "VDynamics|Rope")
	void RebuildRopeAttached(float DistancePerSegment, USceneComponent* StartAttachedTo, FName StartSocket, bool StartSimulateTension, USceneComponent* EndAttachedTo, FName EndSocket, bool EndSimulateTension);

protected:
	void PerformActualRebuild();

protected:
 	int32 DesiredSegmentCount;
 	float DesiredLength;
};
