// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "VICODynamicsSettings.generated.h"

/**
 * Implements the settings for the VICO Dynamics plug-in.
 */
UCLASS(config=Engine)
class UVICODynamicsSettings : public UObject
{
	GENERATED_BODY()

	UVICODynamicsSettings(const FObjectInitializer& ObjectInitializer);

public:
	/** The number of Particles to Reserve memory for. Cannot be changed at runtime. Default is 1024. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	int32 ReserveParticleBudget;

	/** The number of Threads to Reserve in the ThreadPool for the Simulation. Cannot be changed at runtime. Default is 4. NOTE: Only used with *_Parallel builds */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	int32 NumberOfThreadsToUse;

	/** Whether the Particles can collide with each other. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	bool EnableSelfCollision;

	/** Whether the Particles can collide with the PhysX Rigid Bodies. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	bool EnablePhysXSceneCollision;

	/** If true, performs extra Collision Resolution iterations and an extra constraint iteration to improve collision results. NOTE: May impact performance in heavy scenes. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	bool PerformExtraCollisionResolutionLoop;

	/** Whether the Particles can collide with explicitly defined PhysX Rigid Bodies using the VDColliderComponent. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "EnablePhysXSceneCollision"), Category = VICODynamics)
	bool EnableExplicitRigidBodyCollision;

	/** 
	 * The framerate at which the Simulation runs at. Default is 60. 
	 * NOTE: If your game runs at 30fps and this is set to 60, the simulation will do 2 frames worth of work! This can be tweaked at runtime, but may add instability in some cases. 
	 */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	int32 SimulationFramerate;

	/** 
	 * Whether to run the Simulation Tick asynchronously on a worker thread. Default is false.
	 * This allows you to allocate the entire frame budget to the simulation without stalling the game thread.
	 * NOTE: The VDTensionRopeComponent does exhibit strange behavior with this enabled in some cases.
	 */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	bool RunSimulationOnWorkerThread;

	/** The core on which the worker thread will run on, only really applies to Consoles. Default is 1. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, AdvancedDisplay, meta = (EditCondition = "RunSimulationOnWorkerThread"), Category = VICODynamics)
	int32 WorkerThreadCoreAffinity;

	/** The number of Sub-steps per-frame the simulation takes. Default is 3. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	int32 SimulationIterations;

	/** The number of iterations per-substep the constraint solver takes. Default is 8. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	int32 ConstraintIterations;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Constrain to Plane")
	bool ConstrainSimulationTo2DPlane;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Constrain to Plane")
	FVector PlaneOrigin;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Constrain to Plane")
	FVector PlaneNormal;

	/*
	* Whether to Cache the Overlap/Sweep results per-frame to save performance.
	* This will allow the Collision Interface to Cache the initial contacts for each Particle. Any subsequent queries will be fast-tracked using the cache.
	* Performance is gained proportionally to the number of Sub-steps the Simulation performs each frame.
	* Best suited for slower moving collisions, fast moving collisions may begin to be missed if this is enabled.
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Rigid Body Collision")
	uint32 bCachePerFrameResults : 1;

	/*
	* Whether to limit the cache to only collisions with static rigid bodies.
	* This will result in better high speed collision accuracy with dynamic rigid bodies, but at the cost of performance if the scene mainly consists of static colliders.
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Rigid Body Collision")
	uint32 bCacheOnlyCollisionsWithStaticRigidBodies : 1;

	/*
	* Whether to also cache non-hit results to quickly skip particles which had no contact in the current Sub-step and will not be considered for collision until the next frame.
	* This helps performance, but may increase the number of missed collisions with fast moving objects (Particles and/or Rigid Bodies)
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Rigid Body Collision")
	uint32 bCacheNonHitsAlso : 1;

	/** 
	 * If true, draws debug info for collisions with RigidBodies. 
	 * Emerald for Sweep cached contact edge with the Orange line being the resolved link from the cache.
	 * Purple for Swept contact edge with the Red line being the correction required to resolve the collision.
	 */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	uint32 DrawCollisionDebug : 1;

	/*
	 * Prints simulation timing stats to the screen in the upper-right area for extremely low overhead stats.
	 */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	uint32 bShowOnScreenStats : 1;
};

USTRUCT(BlueprintType)
struct FVDSettings
{
	GENERATED_BODY()

	FVDSettings();

	void CopyFrom(const UVICODynamicsSettings* ConfigSettings);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_ReserveParticleBudget : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_NumberOfThreadsToUse : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_EnableSelfCollision : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_EnablePhysXSceneCollision : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_PerformExtraCollisionResolutionLoop : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_EnableExplicitRigidBodyCollision : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_SimulationFramerate : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_RunSimulationOnWorkerThread : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_WorkerThreadCoreAffinity : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_SimulationIterations : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_ConstraintIterations : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_ConstrainSimulationTo2DPlane : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_PlaneOrigin : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_PlaneNormal : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_CachePerFrameResults : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_CacheOnlyCollisionsWithStaticRigidBodies : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_CacheNonHitsAlso : 1;

	/** The number of Particles to Reserve memory for. Cannot be changed at runtime. Default is 1024. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_ReserveParticleBudget))
	int32 ReserveParticleBudget;

	/** The number of Threads to Reserve in the ThreadPool for the Simulation. Cannot be changed at runtime. Default is 4. NOTE: Only used with *_Parallel builds */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_NumberOfThreadsToUse))
	int32 NumberOfThreadsToUse;

	/** Whether the Particles can collide with each other. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_EnableSelfCollision))
	bool EnableSelfCollision;

	/** Whether the Particles can collide with the PhysX Rigid Bodies. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_EnablePhysXSceneCollision))
	bool EnablePhysXSceneCollision;

	/** If true, performs extra Collision Resolution iterations and an extra constraint iteration to improve collision results. NOTE: May impact performance in heavy scenes. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_PerformExtraCollisionResolutionLoop))
	bool PerformExtraCollisionResolutionLoop;

	/** Whether the Particles can collide with explicitly defined PhysX Rigid Bodies using the VDColliderComponent. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_EnableExplicitRigidBodyCollision))
	bool EnableExplicitRigidBodyCollision;

	/**
	* The framerate at which the Simulation runs at. Default is 60.
	* NOTE: If your game runs at 30fps and this is set to 60, the simulation will do 2 frames worth of work! This can be tweaked at runtime, but may add instability in some cases.
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_SimulationFramerate))
	int32 SimulationFramerate;

	/**
	* Whether to run the Simulation Tick asynchronously on a worker thread. Default is false.
	* This allows you to allocate the entire frame budget to the simulation without stalling the game thread.
	* NOTE: The VDTensionRopeComponent does exhibit strange behavior with this enabled in some cases.
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_RunSimulationOnWorkerThread))
	bool RunSimulationOnWorkerThread;

	/** The core on which the worker thread will run on, only really applies to Consoles. Default is 1. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = VICODynamics, meta = (EditCondition = bOverride_WorkerThreadCoreAffinity))
	int32 WorkerThreadCoreAffinity;

	/** The number of Sub-steps per-frame the simulation takes. Default is 3. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_SimulationIterations))
	int32 SimulationIterations;

	/** The number of iterations per-substep the constraint solver takes. Default is 8. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics, meta = (EditCondition = bOverride_ConstraintIterations))
	int32 ConstraintIterations;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Constrain to Plane", meta = (EditCondition = bOverride_ConstrainSimulationTo2DPlane))
	bool ConstrainSimulationTo2DPlane;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Constrain to Plane", meta = (EditCondition = bOverride_PlaneOrigin))
	FVector PlaneOrigin;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Constrain to Plane", meta = (EditCondition = bOverride_PlaneNormal))
	FVector PlaneNormal;

	/*
	* Whether to Cache the Overlap/Sweep results per-frame to save performance.
	* This will allow the Collision Interface to Cache the initial contacts for each Particle. Any subsequent queries will be fast-tracked using the cache.
	* Performance is gained proportionally to the number of Sub-steps the Simulation performs each frame.
	* Best suited for slower moving collisions, fast moving collisions may begin to be missed if this is enabled.
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Rigid Body Collision", meta = (EditCondition = bOverride_CachePerFrameResults))
	uint32 bCachePerFrameResults : 1;

	/*
	* Whether to limit the cache to only collisions with static rigid bodies.
	* This will result in better high speed collision accuracy with dynamic rigid bodies, but at the cost of performance if the scene mainly consists of static colliders.
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Rigid Body Collision", meta = (EditCondition = bOverride_CacheOnlyCollisionsWithStaticRigidBodies))
	uint32 bCacheOnlyCollisionsWithStaticRigidBodies : 1;

	/*
	* Whether to also cache non-hit results to quickly skip particles which had no contact in the current Sub-step and will not be considered for collision until the next frame.
	* This helps performance, but may increase the number of missed collisions with fast moving objects (Particles and/or Rigid Bodies)
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "VICODynamics|Rigid Body Collision", meta = (EditCondition = bOverride_CacheNonHitsAlso))
	uint32 bCacheNonHitsAlso : 1;

	/**
	* If true, draws debug info for collisions with RigidBodies.
	* Emerald for Sweep cached contact edge with the Orange line being the resolved link from the cache.
	* Purple for Swept contact edge with the Red line being the correction required to resolve the collision.
	*/
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	uint32 DrawCollisionDebug : 1;

	/*
	 * Prints simulation timing stats to the screen in the upper-right area for extremely low overhead stats.
	 */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = VICODynamics)
	uint32 bShowOnScreenStats : 1;
};