// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Simulation.h"
#include "SimulatedObject.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/AllowWindowsPlatformAtomics.h"
#endif

#include "ThreadUtilities.h"

#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformAtomics.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#include "VDSimulation.h"
#include "VDSimulatedObject.h"
#include "VDSimulatedParticle.h"

#include "PhysicsPublic.h"

#include <unordered_set>
#include "WorldCollision.h"

using FOverlapCacheKey = const vicoDynamics::FSimulatedParticle*;
// struct FOverlapCacheKey
// {
// 	FVector Location;
// 	const vicoDynamics::FSimulatedParticle* Particle;
// };
//using FOverlapCacheKey = TTuple<FVector, const vicoDynamics::FSimulatedParticle*>;
struct FOverlapCacheEntry
{
	PxRigidActor* Actor;
	PxShape* Shape;
};
//using FOverlapCacheEntry = TTuple<PxRigidActor*, PxShape*>;

using FSweepCacheKey = const vicoDynamics::FSimulatedParticle*;
//using FSweepCacheKey = TTuple<float, FVector, FVector, const vicoDynamics::FSimulatedParticle*>;
struct FSweepCacheEntry
{
	FSweepCacheEntry() = default;
	FSweepCacheEntry(FSweepCacheEntry&& Other) = default;

	PxRigidActor* Actor;
	PxShape* Shape;
	vicoDynamics::FVector3Data Position;
	vicoDynamics::FVector3Data Normal;
	float Depth;
};
//using FSweepCacheEntry = TTuple<PxRigidActor*, PxShape*, vicoDynamics::FVector3Data, vicoDynamics::FVector3Data, float>;

class FVDCollisionQueryInterface : public vicoDynamics::FCollisionQueryInterface
{
public:
	FVDCollisionQueryInterface(UVDSimulation* InSimulation, bool CachePerFrameResults = true, bool CacheOnlyCollisionsWithStaticRigidBodies = false, bool CacheNonHitsAsWell = true);
	virtual bool OverlapSphere(float Radius, const vicoDynamics::FVector3Data& Location, vicoDynamics::FQueryResults* Results, 
							   const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject) override;
	virtual bool SweepSphere(float Radius, const vicoDynamics::FVector3Data& StartLocation, const vicoDynamics::FVector3Data& EndLocation, vicoDynamics::FQueryResults* Results, 
							 const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject) override;
	virtual void LockSceneRead() override;
	virtual void LockSceneWrite() override;
	virtual void UnlockSceneRead() override;
	virtual void UnlockSceneWrite() override;

	void ResetCache();

protected:
	FORCEINLINE_DEBUGGABLE bool OverlapWithExplicitRigidBodies(UBaseVDComponent* VDComp, UVDSimulatedObject* VDSimObj, float Radius, const vicoDynamics::FVector3Data& Location, vicoDynamics::FQueryResults* Results, 
															   const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject);
	FORCEINLINE_DEBUGGABLE bool SweepAgainstExplicitRigidBodies(UBaseVDComponent* VDComp, UVDSimulatedObject* VDSimObj, float Radius, const vicoDynamics::FVector3Data& StartLocation, const vicoDynamics::FVector3Data& EndLocation, vicoDynamics::FQueryResults* Results,
																const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject);

private:
	TWeakObjectPtr<UVDSimulation> Simulation;

public:
	/* 
	 * Whether to Cache the Overlap/Sweep results per-frame to save performance. 
	 * This will allow the Collision Interface to Cache the initial contacts for each Particle. Any subsequent queries will be fast-tracked using the cache.
	 * Performance is gained proportionally to the number of Sub-steps the Simulation performs each frame. 
	 * Best suited for slower moving collisions, fast moving collisions may begin to be missed if this is enabled.
	 */
	uint32 bCachePerFrameResults : 1;

	/*
	 * Whether to limit the cache to only collisions with static rigid bodies. 
	 * This will result in better high speed collision accuracy with dynamic rigid bodies, but at the cost of performance if the scene mainly consists of static colliders.
	 */
	uint32 bCacheOnlyCollisionsWithStaticRigidBodies : 1;

	/*
	 * Whether to also cache non-hit results to quickly skip particles which had no contact in the current Sub-step and will not be considered for collision until the next frame.
	 * This helps performance, but may increase the number of missed collisions with fast moving objects (Particles and/or Rigid Bodies)
	 */
	uint32 bCacheNonHitsAlso : 1;
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	uint32 DrawCollisionDebug : 1;
#endif

private:
	FCollisionShape CollisionShape;
	FCollisionQueryParams ColParams;

	TMap<FOverlapCacheKey, FOverlapCacheEntry> OverlapCache;
	TMap<FSweepCacheKey, FSweepCacheEntry> SweepCache;

	mutable vicoDynamics::threading::FReadWriteLock OverlapCacheCritSec;
	mutable vicoDynamics::threading::FReadWriteLock SweepCacheCritSec;

	friend class UVDSimulation;
};