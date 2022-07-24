// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "DataContainers.h"
#include "VICODynamicsSettings.h"
#include "Engine/EngineBaseTypes.h"
#include "PhysicsPublic.h"
#include "HAL/Runnable.h"
#include "Engine/World.h"
#include "VDSimulation.generated.h"

DECLARE_STATS_GROUP(TEXT("VICODynamics Stats"), STATGROUP_VICODynamics, STATCAT_VICODynamics);
DECLARE_DWORD_COUNTER_STAT(TEXT("Particles"), STAT_NumParticles, STATGROUP_VICODynamics);
DECLARE_DWORD_COUNTER_STAT(TEXT("Constraints"), STAT_NumConstraints, STATGROUP_VICODynamics);
DECLARE_CYCLE_STAT(TEXT("VDSimulation Tick"), STAT_VDSimTick, STATGROUP_VICODynamics);
DECLARE_CYCLE_STAT(TEXT("VDSimulation Worker Thread Tick"), STAT_VDSimWorkerTick, STATGROUP_VICODynamics);
DECLARE_CYCLE_STAT(TEXT("Simulation Tick"), STAT_SimTick, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Simulation MS"), STAT_Simulation, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Velocity MS"), STAT_Velocity, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Collision MS"), STAT_Collision, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Particle Collisision Broad MS"), STAT_ParticleCollisionBroad, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Particle Collisision Narrow MS"), STAT_ParticleCollisionNarrow, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("RigidBody Collisision MS"), STAT_RigidBodyCollision, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Location and Velocity Correction MS"), STAT_LocationVelocityCorrection, STATGROUP_VICODynamics);
DECLARE_FLOAT_COUNTER_STAT(TEXT("Constraints MS"), STAT_Constraints, STATGROUP_VICODynamics);
DECLARE_CYCLE_STAT(TEXT("VD Sync"), STAT_VDSync, STATGROUP_VICODynamics);
DECLARE_CYCLE_STAT(TEXT("VDSimulation Query Overlap"), STAT_VDSimQueryOverlap, STATGROUP_VICODynamics);

DECLARE_STATS_GROUP(TEXT("VICODynamics Collision Interface Stats"), STATGROUP_VICODynamicsCollision, STATCAT_VICODynamics);
DECLARE_CYCLE_STAT(TEXT("CI Sweep Cache Check"), STAT_CISweepCacheCheck, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Sweep Cache Hit"), STAT_CISweepCacheHit, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Overlap Cache Hit"), STAT_CIOverlapCacheHit, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Sweep"), STAT_CISweep, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Explicit Sweep"), STAT_CIExplicitSweep, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Explicit Sweep PxGeom"), STAT_CIExplicitSweepPxGeom, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Overlap"), STAT_CIOverlap, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Explicit Overlap"), STAT_CIExplicitOverlap, STATGROUP_VICODynamicsCollision);
DECLARE_CYCLE_STAT(TEXT("CI Explicit Overlap PxGeom"), STAT_CIExplicitOverlapPxGeom, STATGROUP_VICODynamicsCollision);

DECLARE_CYCLE_STAT(TEXT("CI CacheReset"), STAT_CICacheReset, STATGROUP_VICODynamicsCollision);

DECLARE_STATS_GROUP(TEXT("VICODynamics Rendering Stats"), STATGROUP_VICODynamicsRendering, STATCAT_VICODynamics);


namespace vicoDynamics
{
	class FSimulation;
	class FSimulatedObject;
	struct FSimulatedParticle;
}

class UVDSimulation;
class FVDCollisionQueryInterface;
class UBaseVDComponent;
class UVDWindDirectionalSourceComponent;

class FSimulationThreadWorker : public FRunnable
{
public:
	FSimulationThreadWorker(UVDSimulation* Simulation, uint32 CoreAffinity);
	virtual ~FSimulationThreadWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void EnsureCompletion();
	void TerminateImmediately();

	void SetWorldRef(UWorld* InWorld)
	{
		WorldRef.Reset();
		WorldRef = InWorld;
	}

	UWorld* GetWorldRef() { return WorldRef.Get(); }
	
private:
	UVDSimulation* VDSimulation;
	FRunnableThread* Thread;
	TWeakObjectPtr<UWorld> WorldRef;

public:
	bool FrameComplete;

	friend class UVDSimulation;
};

USTRUCT()
struct FVDSimulationTickFunction : public FTickFunction
{
	GENERATED_BODY();
	
	/**
	* Abstract function actually execute the tick.
	* @param DeltaTime - frame time to advance, in seconds
	* @param TickType - kind of tick for this frame
	* @param CurrentThread - thread we are executing on, useful to pass along as new tasks are created
	* @param MyCompletionGraphEvent - completion event for this task. Useful for holding the completion of this task until certain child tasks are complete.
	**/
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;

	/** Abstract function to describe this tick. Used to print messages about illegal cycles in the dependency graph **/
	virtual FString DiagnosticMessage();

	UVDSimulation* Target;
};

template<>
struct TStructOpsTypeTraits<FVDSimulationTickFunction> : public TStructOpsTypeTraitsBase2<FVDSimulationTickFunction>
{
	enum
	{
		WithCopy = false
	};
};

// Notifies Components when to Sync up the particle data and gives the Simulation access to the Owning Component
struct IVICODynamicsSimulated
{
	virtual void Sync() {}
	virtual void Release() {}
};

UCLASS(BlueprintType, ClassGroup = VDynamics)
class VICODYNAMICSPLUGIN_API UVDSimulation : public UObject
{
	GENERATED_BODY()

public:
	UVDSimulation();

	vicoDynamics::FSimulation* GetSimulation();
	
	void Register(IVICODynamicsSimulated* Simulated);
	void Unregister(IVICODynamicsSimulated* Simulated);

	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void AddExplicitColliderMapping(UBaseVDComponent* VDComponent, UPrimitiveComponent* Collider, bool IgnoreConvexShapes);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void RemoveExplicitColliderMapping(UBaseVDComponent* VDComponent, UPrimitiveComponent* Collider);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void RemoveAllExplicitColliderMappings(UBaseVDComponent* VDComponent);

	void Tick(float DeltaTime);

	void BeginDestroy() override;
	void UpdateSimStats();

	UFUNCTION(BlueprintPure, Category = VICODynamics)
	bool GetIsRunningOnWorkerThread() const	{ return RunOnWorkerThread;	}
	UFUNCTION(BlueprintPure, Category = VICODynamics)
	bool GetIsExplicitRigidBodyCollisionEnabled() const { return ExplicitRigidBodyCollisionEnabled; }
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetIsExplicitRigidBodyCollisionEnabled(bool IsEnabled);
	UFUNCTION(BlueprintPure, Category = VICODynamics)
	FVDSettings GetSettings() { return Settings; }

	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetSubstepCount(int32 SubstepCount);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetConstraintIterationCount(int32 ConstraintIterationCount);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetFramerate(int32 Framerate);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetSelfCollisionEnabled(bool IsEnabled);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetPhysXSceneCollisionEnabled(bool IsEnabled);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetPerformExtraCollisionResolutionLoop(bool IsEnabled);
	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	void SetPerFrameCollisionCacheEnable(bool IsEnabled);
	UFUNCTION(BlueprintCallable, Category = "VICODynamics|Constrain to Plane")
	void SetConstrainSimulationTo2DPlane(bool IsEnabled);
	UFUNCTION(BlueprintCallable, Category = "VICODynamics|Constrain to Plane")
	void SetConstrainPlaneOrigin(FVector Origin);
	UFUNCTION(BlueprintCallable, Category = "VICODynamics|Constrain to Plane")
	void SetConstrainPlaneNormal(FVector Normal);
	
	void AddWindSource(UVDWindDirectionalSourceComponent* WindSource);
	void RemoveWindSource(UVDWindDirectionalSourceComponent* WindSource);

	UFUNCTION(BlueprintCallable, Category = "VDynamics|Wind")
	void GetDirectionalWindParameters(UBaseVDComponent* VDComponent, FVector& OutDirection, float& OutSpeed, float& OutGust) const;

	UFUNCTION(BlueprintCallable, Category = VICODynamics, meta = (DisplayName = "QueryBoxOverlaps"))
	bool k2QueryBoxOverlaps(FVector Location, FVector HalfExtents, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore, int32 MaxOverlaps = 1000);
	bool QueryBoxOverlaps(FVector Location, FVector HalfExtents, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore = TArray<UVDSimulatedObject*>(), int32 MaxOverlaps = 1000);
	UFUNCTION(BlueprintCallable, Category = VICODynamics, meta = (DisplayName = "QuerySphereOverlaps"))
	bool k2QuerySphereOverlaps(FVector Location, float Radius, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore, int32 MaxOverlaps = 1000);
	bool QuerySphereOverlaps(FVector Location, float Radius, TArray<UVDSimulatedParticle*>& Overlaps, const TArray<UVDSimulatedObject*>& SimObjectsToIgnore = TArray<UVDSimulatedObject*>(), int32 MaxOverlaps = 1000);

protected:
	void OnEndLevelPlay(UBaseVDComponent* Notifier);

private:
	vicoDynamics::FSimulation* Simulation;
	bool RunOnWorkerThread;

	TArray<IVICODynamicsSimulated*> RegisteredSimulatedObjects;
	TArray<IVICODynamicsSimulated*> SimulatedObjectsToRemove;

	FVDSimulationTickFunction TickFunction;
	uint32 bTickRegistered : 1;
	uint32 bReportStatsOnNextTick : 1;
	uint32 bWorkerThreadNotifiedOfEndLevel : 1;

	//UPROPERTY()
	UWorld* CurWorld;

	uint32 bShowOnScreenStats : 1;

	FVDCollisionQueryInterface* CollisionInterface;	
	bool ExplicitRigidBodyCollisionEnabled;
	FCriticalSection ColliderMappingCritSec;

	FVDSettings Settings;

	TSharedPtr<FSimulationThreadWorker> SimulationThreadWorker;
	FCriticalSection SimulationSettingsCritSec;

	UPROPERTY()
	TArray<UVDWindDirectionalSourceComponent*> WindSources;

	friend class FSimulationThreadWorker;
	friend class FVDCollisionQueryInterface;
	friend class UBaseVDComponent;
};

// Quick utility functions to convert between UE4 FVector and VICODynamics FVector3Data

FORCEINLINE FVector VDVectorToUVector(const vicoDynamics::FVector3Data& VDVec);
FORCEINLINE vicoDynamics::FVector3Data UVectorToVDVector(const FVector& UVec);