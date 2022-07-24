// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Components/MeshComponent.h"

#include "VDSimulation.h"
#include "VDSimulatedParticle.h"
#include "VDSimulatedObject.h"
#include "VDColliderInfo.h"

#include "ParticleConstraints.h"

#include "Containers/Queue.h"
#include "Engine/World.h"

#include "BaseVDComponent.generated.h"

class UBaseVDComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FVDCollisionDelegate, UBaseVDComponent*, VDComponent, UPrimitiveComponent*, OtherComponent, FVector, HitLocation, FVector, HitNormal, UVDSimulatedParticle*, Particle);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
struct FVDDebugRenderData
{
	uint32 IsFree : 1;
	float Radius;
};
#endif

struct FVDProxyParticle
{
	FVector Position;
	FVector ProjectedPosition;
	float InvMass;
	int32 ParticleIndex;
};

USTRUCT()
struct FVDCollision
{
	GENERATED_BODY()
	FVDCollision() : OtherComp(nullptr), HitLocation(ForceInitToZero), HitNormal(ForceInitToZero)
	{}

	TWeakObjectPtr<UPrimitiveComponent> OtherComp;
	FVector HitLocation;
	FVector HitNormal;
};

struct VICODYNAMICSPLUGIN_API FVDTensionConstraint : public vicoDynamics::FExternalConstraint
{
	FVDTensionConstraint();
	virtual ~FVDTensionConstraint();

	void Init(UBaseVDComponent* Component, int32 ParticleIndex, FVDParticleAttachment* InAttachment, float InRestDistance, float InTensionForce, float InDampening);
	virtual void Solve(float DeltaTime) override;
	void ApplyTensionToAttachment(float DeltaTime, float Dialation);
	
private:
	FVector CalculateSpringForceForTimeStep(float CompMass, float SimDT, FVector MLoc, float DeltaTime, UPrimitiveComponent* Comp, UBaseVDComponent* OwnerVDComp, FVector PrevForce = FVector::ZeroVector);
	void ResetProxyParticle();

private:
	FWeakObjectPtr OwnerComp;
	bool CorrSet;
public:
	FVDProxyParticle ProxyParticle;
	FVDParticleAttachment* Attachment;
	float RestDistance;
	float TensionForce;
	float Dampening;
	bool ShouldSnapToProxyForRender;

	FVector LastAppliedForce;
};

struct VICODYNAMICSPLUGIN_API FVDForce
{
	FVDForce() : Origin(0.f), Radius(0.f), Strength(0.f), Falloff(RIF_Constant)
	{}
	FVDForce(FVector origin, float radius, float strength, enum ERadialImpulseFalloff falloff) : Origin(origin), Radius(radius), Strength(strength), Falloff(falloff)
	{}

	FVector Origin;
	float Radius;
	float Strength;
	ERadialImpulseFalloff Falloff;
};

USTRUCT(BlueprintType)
struct VICODYNAMICSPLUGIN_API FVDCachedParticleStartLocation
{
	GENERATED_BODY();

	FVDCachedParticleStartLocation() : Location(0.f), bIsAttached(false)
	{}
	FVDCachedParticleStartLocation(FVector location, bool isAttached) : Location(location), bIsAttached(isAttached)
	{}

	UPROPERTY(EditAnywhere, Category = "VDynamics")
	FVector Location;
	UPROPERTY(EditAnywhere, Category = "VDynamics")
	bool bIsAttached;
};

/** A notify of when the Simulation Data has been created */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVDOnSimulationDataCreated);

/** A notify of when the Simulation has finished its Tick */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVDOnSimulationSync);

/**
 * Base class for all VICODynamics components.
 */
UCLASS(abstract, DefaultToInstanced)
class VICODYNAMICSPLUGIN_API UBaseVDComponent : public UMeshComponent, public IVICODynamicsSimulated
{
public:
	GENERATED_BODY()

	UBaseVDComponent(const FObjectInitializer& ObjectInitializer);
	
	// Begin UObject interface.
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual bool CanEditChange(const UProperty* InProperty) const override;
#endif // WITH_EDITOR
	virtual void PostLoad() override;
	// End UObject interface.

	// Begin UActorComponent interface.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual TStructOnScope<FActorComponentInstanceData> GetComponentInstanceData() const override;
	// End UActorComponent interface.

	// Begin UPrimitiveComponent interface.
	virtual void AddRadialForce(FVector Origin, float Radius, float Strength, enum ERadialImpulseFalloff Falloff, bool bAccelChange = false) override;
	// End UPrimitiveComponent interface.

	// Begin IVICODynamicsSimulated Interface
	/** Signals that a simulate tick has finished and it is safe to access/modify Particle and/or Constraint data */
	virtual void Sync() override;
	/** Perform any Simulation related clean up */
	virtual void Release() override;
	// End IVICODynamicsSimulated Interface

	virtual void RegisterWithSimulation();
	virtual void UnregisterWithSimulation();

	virtual void ApproximateWindDrag(const FVector& Direction, float Speed);

	UVDSimulatedObject* GetVDSimObject() { return VDSimObject; }
	const TArray<UVDSimulatedParticle*>& GetParticlesArray() const { return Particles; }

	virtual void CreateTensionConstraintsFromAttachment(FVDParticleAttachment& Attachment);
	virtual void CreateParticleConstraintsFromAttachment(FVDParticleAttachment& Attachment);

	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	virtual void AttachToOverlappingRigidBodies();

#if WITH_EDITOR
	void UpdateCollisionProperty();
#endif // WITH_EDITOR

	virtual const TArray<FVDParticleInfo>* GetParticleBuildInfo() const;

	UFUNCTION(BlueprintPure, Category = "VDynamics")
	int32 GetTensionConstraintCount() const { return TensionConstraints.Num(); }
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	FVector GetLastAppliedForceFromTensionConstraint(int32 TensionConstraintIndex = 0) const;

protected:
	FVector ResolveAttachmentLocation(const FVDParticleAttachment& Attachment);

	void ApplyForce(FVector Origin, float Radius, float Strength, enum ERadialImpulseFalloff Falloff);

	FORCEINLINE static float CalculateIncidentAngleToWindVector(const FVector& WindDirNormal, const FVector& SurfaceNormal);
	FORCEINLINE bool IsOccludedFromWind(const FVector& Location, const FVector& WindDir) const;

	void AttachToOverlappingRigidBodiesInternal(int32 ParticleCount);

	/* Signals that a Simulate Tick has finished and it is safe to access/modify Particle and/or Constraint data. NOTE: This is not tied to the games Tick! */
	UFUNCTION(BlueprintImplementableEvent, Category = "VDynamics")
	void OnSync();

	void AddPendingCollisionNotification(UVDSimulatedParticle* Particle, UPrimitiveComponent* OtherComp, const FVector& HitLocation, const FVector& HitNormal);
	
public:
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetAffectedByGlobalWind(bool IsAffected);
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetUseExplicitRigidBodyCollision(bool UseExplicitCollision);
	
#if WITH_EDITORONLY_DATA
	bool GetUseCachedLocations() const { return bUseLocationCache; }
	const TArray<FVDCachedParticleStartLocation>& GetCachedLocations() const { return CachedStartLocations; }
#endif
	
protected:
	/** The Simulation this Component is using */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "VDynamics")
	UVDSimulation* SimulationInstance;

	UPROPERTY(Transient, BlueprintReadOnly, DisplayName = "SimulationObject", Category = "VDynamics", meta = (ShowOnlyInnerProperties))
	UVDSimulatedObject* VDSimObject;

	/** Particles array */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "VDynamics")
	TArray<UVDSimulatedParticle*> Particles;

	/** Currently only used with Rigid Body collisions. To edit at runtime, access individual Particles and call SetFrictionCoefficient() */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VDynamics")
	float FrictionCoefficient;

	/** Currently only used with Rigid Body collisions. To edit at runtime, access individual Particles and call SetRestitutionCoefficient() */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VDynamics")
	float RestitutionCoefficient;

	/** Collision Settings for Editing */
	UPROPERTY(EditAnywhere, Category = "VDynamics", meta = (ShowOnlyInnerProperties))
	struct FVDCollisionSettings CollisionSettings;

public:
	/** The radius of the particles that make up the rope. Used for collision and drag simulation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ParticleRadius;
		
	/** Particles embedded in shapes at level start up will be permanently attached to them */
	UPROPERTY(EditAnywhere, Category = "Attachment", meta = (EditCondition = "!ManuallyAttachParticles"))
	bool AttachToRigids;

	/** Particles embedded in other Particles (of a different VD Component) at level start up will be attached to them using Constraints. NOTE: Self Collision must be enabled in Project Settings for this to work. */
	UPROPERTY(EditAnywhere, Category = "Attachment", meta = (EditCondition = "!ManuallyAttachParticles"))
	bool AttachToParticles;

	/** Stiffness of the created Particle attachment Constraints. Only used when AttachToParticles is true */
	UPROPERTY(EditAnywhere, Category = "Attachment", meta = (EditCondition = "!ManuallyAttachParticles"))
	float AttachToParticlesConstraintStiffness;

	/** Only used with Ropes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attachment")
	float EndOffset;

	/** The factor by which to inflate the Particle Radius when detecting overlapping Rigids (if AttachToRigids is true) and/or Particles (if AttachToParticles is true) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attachment", meta = (EditCondition = "!ManuallyAttachParticles"))
	float DetectionRadiusInflationFactor;

	/** If true, Tension constraints will be created if the Rigid is Simulating Physics */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attachment", meta = (EditCondition = "AttachToRigids"))
	bool CreateTensionConstraintsForSimulatingRigids;

	/** If true, Particles attached via Tension constraints will be snapped to the Simulating Component during Rendering. Otherwise the position is interpolated between the Particle and the attachment point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	bool bSnapToSimulatingAttachment;

	UPROPERTY(EditAnywhere, Category = "Attachment")
	bool ManuallyAttachParticles;

	/** Collection of manually created Particle Attachments */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attachment", meta = (EditCondition = "ManuallyAttachParticles"))
	TArray<FVDParticleAttachment> ParticleAttachments;

	/** The max amount of tension that is exerted when attached to Simulating Actors. Must be greater than 0 */
	UPROPERTY(EditAnywhere, Category = "Attachment", meta = (ClampMin = "0.0001", UIMin = "0.0001"))
	float TensionForce;

	/** The amount of dampening the tension constraints receive. [0-1] */
	UPROPERTY(EditAnywhere, Category = "Attachment", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float TensionDampening;

	/** If enabled, Particle locations will be adjusted based on attachments. For example, if Particle 1 is attached at an offset of {10, 0, 0} all Particles after Particle 1 will have their start location offset by {10, 0, 0} as well */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attachment")
	uint32 AdjustOtherParticleLocationsBasedOnAttachments : 1;

	/** If enabled, the Simulated Object will be affected by Wind Source Actors placed in the World. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics")
	uint32 AffectedByGlobalWind : 1;

	/** If enabled in Project Settings and here, uses an explicitly defined list of PrimitiveComponents with which this Component can possibly collide with. Use the VDColliderComponent to mark Actor Components as colliders with this component. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VDynamics")
	uint32 UseExplicitRigidBodyCollision : 1;
		
	/** 
	 * If AffectedByGlobalWind is enabled, the Drag Coefficient value will be scaled by this value in relation to the angle against the wind vector. 
	 * In short, when the rope or cloth part is perpendicular to the wind; the drag coefficient will be scaled by this value and by the relative angle against the wind. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (EditCondition = "AffectedByGlobalWind"))
	float WindResistance;

	/** If not empty, only the wind sources in this array affect this component */
	UPROPERTY(EditAnywhere, Category = "VDynamics", meta = (EditCondition = "AffectedByGlobalWind"))
	TArray<FComponentReference> AffectedByWindSources;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (EditCondition = "AffectedByGlobalWind"))
	bool CheckForWindOccluders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (EditCondition = "CheckForWindOccluders"))
	float MaxWindOccluderDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (EditCondition = "CheckForWindOccluders"))
	struct FVDCollisionSettings WindOccluderTraceSettings;
	
	/* Signals that Simulation Data (Particles/Constraints) have been created and it is safe to access/modify/create/destroy Particles and/or Constraints. */
	UPROPERTY(BlueprintAssignable, Category = "VDynamics")
	FVDOnSimulationDataCreated OnSimulationDataCreated;

	/* Signals that a Simulate Tick has finished and it is safe to access/modify Particle and/or Constraint data. NOTE: This is not tied to the games Tick! */
	UPROPERTY(BlueprintAssignable, Category = "VDynamics")
	FVDOnSimulationSync OnSimulationSync;
	
	/** Renders a wireframe sphere for each particle with correct radius. Red if the Particle is Fixed/Attached. NOTE: Code paths disabled in Shipping builds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics|Debug")
	uint32 ShowParticles : 1;

protected:
	/** Scales global Simulation Gravity on a Per-Object basis. */
	UPROPERTY(EditAnywhere, Category = "VDynamics", AdvancedDisplay)
	float GravityScale;
	/** Allows for skipping iterations once a constraint has reached this distance. Setting this too high will result in jerky constraints. Default is .007f */
	UPROPERTY(EditAnywhere, Category = "VDynamics", AdvancedDisplay)
	float MinConstraintDistance;
	/** Allows for disabling simulation of this Component when not being rendered. */
	UPROPERTY(EditAnywhere, Category = "VDynamics", AdvancedDisplay)
	bool SimulateOnlyWhenRendered;
	/** The amount of time since this Component stopped being Rendered, in Seconds, until it stop Simulating. The reverse is immediate. */
	UPROPERTY(EditAnywhere, Category = "VDynamics", AdvancedDisplay, meta = (EditCondition = "SimulateOnlyWhenRendered"))
	float SimulationToggleThreshold;
	/**
	 * If true, collisions with other soft-bodies will also be resolved. This will affect performance.
	 * NOTE: Incorrect Particle Radius values will create artifacts.
	 */
	UPROPERTY(EditAnywhere, Category = "VDynamics")
	uint32 EnableParticleToParticleCollisions : 1;
	/** 
	 * If true, collisions within this soft-body will also be resolved. This will affect performance.
	 * NOTE: Incorrect Particle Radius values will create artifacts.
	 */
	UPROPERTY(EditAnywhere, Category = "VDynamics", AdvancedDisplay, meta = (EditCondition = "EnableParticleToParticleCollisions"))
	uint32 ConsiderInnerCollisions : 1;
	/** If true, the relative velocity is not taken into account during RigidBody collision. ADVANCED OPTION, helps in some edge cases where kinematic RigidBodies move into soft-bodies causing inter-penetration. */
	UPROPERTY(EditAnywhere, Category = "VDynamics", AdvancedDisplay)
	uint32 SkipRelativeVelocityCheckDuringRigidBodyCollisions : 1;
	
	/* If true, this component will register with VDSimulation in OnRegister and unregister in OnUnregister automatically */
	uint32 bHandleRegistrationWithSimulation : 1;
	uint32 bRegisteredWithSimulation : 1;
	uint32 bShouldToggleSimulationOnNextSync : 1;
	uint32 bIsSimulating : 1;
	uint32 bSimObjCachedCollisionEnabled : 1;

public:
#if WITH_EDITORONLY_DATA
	/** If true, renders the Index of each Particle in the Viewport. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics|Debug")
	uint32 bShowParticleIndices : 1;
	/** If true, renders the Constraints in the Viewport (Only during Simulate). Blue = Stretch Constraints, Violet = Bend Constraints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics|Debug")
	uint32 bVisualizeConstraints : 1;
#endif // WITH_EDITORONLY_DATA

	/** If true, triggers the OnVDCollisionEvent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics")
	uint32 bNotifyOnVDCollision : 1;
	/** Notifies of Particle Collisions with Rigid Bodies only */
	UPROPERTY(BlueprintAssignable, Category = "VDynamics")
	FVDCollisionDelegate OnVDCollisionEvent;

protected:
	UPROPERTY()
	TMap<UVDSimulatedParticle*, FVDCollision> PendingCollisionNotifications;

	TArray<FExplicitColliderInfo> ExplicitColliders;

	TArray<TSharedPtr<FVDTensionConstraint>> TensionConstraints;
	UPROPERTY()
	TArray<UVDParticleSpringConstraint*> ConstraintsToOtherParticles;

	TQueue<FVDForce> ForcesToApply;

	TQueue<FVDParticleAttachment> AttachmentsToParticlesPendingProcessing;

#if WITH_EDITORONLY_DATA
	/** Functions as a button during Simulate-in-Editor to fill the CachedStartLocations array with the Particles' current locations. */
	UPROPERTY(EditAnywhere, Category = "VDynamics")
	uint32 bFillLocationCache : 1;
#endif // WITH_EDITORONLY_DATA

	/** If true, Particle start locations will be set according to the CachedStartLocations array. Use FillLocationCache during Simulate-in-Editor to fill that array. */
	UPROPERTY(EditAnywhere, Category = "VDynamics")
	uint32 bUseLocationCache : 1;
	/** If true, AttachToRigids will be evaluated after cached start locations have been applied to the Particles */
	UPROPERTY(EditAnywhere, Category = "VDynamics", meta = (EditCondition = "bUseLocationCache"))
	uint32 bApplyCachedStartLocationBeforeAttachToRigids : 1;
	/** The cached, per-Particle, start location entries. Use FillLocationCache during Simulate-in-Editor to fill this array. */
	UPROPERTY(EditAnywhere, Category = "VDynamics")
	TArray<FVDCachedParticleStartLocation> CachedStartLocations;

	friend class UVDSimulation;
	friend class FVDCollisionQueryInterface;
	friend class UVDSimulatedParticle;
	friend class UVDParticleSpringConstraint;
	friend struct FVDTensionConstraint;
};

struct FBaseVDComponentInstanceData : public FPrimitiveComponentInstanceData
{
public:
	FBaseVDComponentInstanceData(const UBaseVDComponent* SourceComponent);
	virtual ~FBaseVDComponentInstanceData()
	{}

	virtual void ApplyToComponent(UActorComponent* Component, const ECacheApplyPhase CacheApplyPhase) override;

	bool ContainsData() const;
};

FORCEINLINE float UBaseVDComponent::CalculateIncidentAngleToWindVector(const FVector& WindDirNormal, const FVector& SurfaceNormal)
{
	static const float INVERSE_PI = 0.6366197f; // 1 / (PI/2)

	const float Angle = FMath::Acos(WindDirNormal | SurfaceNormal); // Find radians between the two vectors [0-PI/2], we assume that both are normalized and contain absolute values for our purposes
	const float NormalizedAngle = Angle * INVERSE_PI; // Make it [0-1]
	return FMath::InterpEaseInOut(0.f, 1.f, NormalizedAngle, 3.f); // We do a cubic interpolation to better approximate the increase of wind resistance on a flat perpendicular surface
}

FORCEINLINE bool UBaseVDComponent::IsOccludedFromWind(const FVector& Location, const FVector& WindDir) const
{
	auto World = GetWorld();
	if (!World) return false;

	//World->DebugDrawTraceTag = TEXT("WindOcclusionTrace");

	const auto TraceHit = World->LineTraceTestByChannel(Location, Location + (-MaxWindOccluderDistance * WindDir)
		, WindOccluderTraceSettings.ObjectType, FCollisionQueryParams(TEXT("WindOcclusionTrace"), false, nullptr)
		, FCollisionResponseParams(WindOccluderTraceSettings.CollisionResponse.GetResponseContainer()));

	return TraceHit;
}