// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "VDSimulatedParticle.h"
#include "VDParticleSpringConstraint.h"
#include "VDSimulatedObject.generated.h"

namespace vicoDynamics
{
	class FSimulatedObject;
}

USTRUCT(BlueprintType)
struct VICODYNAMICSPLUGIN_API FVDCollisionSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "VDynamics|Collision")
	TEnumAsByte<enum ECollisionChannel> ObjectType;

	UPROPERTY(EditAnywhere, Category = "VDynamics|Collision")
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;

	UPROPERTY(EditAnywhere, Category = "VDynamics|Collision")
	struct FCollisionResponse CollisionResponse;

	UPROPERTY(EditAnywhere, Category = "VDynamics|Collision")
	FName CollisionProfileName;
};

UCLASS(BlueprintType, ClassGroup = VDynamics, EditInlineNew, DefaultToInstanced)
class VICODYNAMICSPLUGIN_API UVDSimulatedObject : public UObject
{
public:
	GENERATED_BODY()

	UVDSimulatedObject(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginDestroy() override;

	void ReleaseNow();

	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	UVDSimulatedParticle* CreateParticle(const FVDParticleInfo& ParticleInfo);
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	UVDParticleSpringConstraint* CreateSpringConstraint(UVDSimulatedParticle* Particle1, UVDSimulatedParticle* Particle2, float RestDistance, float Stiffness);

	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void DestroyParticle(UVDSimulatedParticle* Particle);
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void DestroyConstraint(UVDParticleSpringConstraint* Constraint);

	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	TArray<UVDSimulatedParticle*> GetParticles();
	int32 GetParticles(TArray<UVDSimulatedParticle*>& OutParticles);
	
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	TArray<UVDParticleSpringConstraint*> GetSpringConstraints();
	int32 GetSpringConstraints(TArray<UVDParticleSpringConstraint*>& OutSpringConstraints);

	/** Destroys all associated Particles and Constraints but keeps the SimObject instance */
	void RemoveFromSimulation();

	UFUNCTION(BlueprintPure, Category = "VDynamics")
	float GetGravityScale() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetGravityScale(float Scale);

	UFUNCTION(BlueprintPure, Category = "VDynamics")
	FVector GetAffectiveWindVector() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetAffectiveWindVector(FVector NewWindVector);
	
	UFUNCTION(BlueprintPure, Category = "VDynamics")
	bool GetCollisionEnabled() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetCollisionEnabled(bool Enabled);

	UFUNCTION(BlueprintPure, Category = "VDynamics")
	float GetMinimumConstraintSatisfactionDistance() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetMinimumConstraintSatisfactionDistance(float MinDistance);

	/** If true, the relative velocity is not taken into account during RigidBody collision. ADVANCED OPTION, helps in some edge cases where kinematic RigidBodies move into soft-bodies causing inter-penetration. */
	UFUNCTION(BlueprintPure, Category = "VDynamics")
	bool GetSkipRelativeVelocityCheckDuringCollision() const;
	/** If true, the relative velocity is not taken into account during RigidBody collision. ADVANCED OPTION, helps in some edge cases where kinematic RigidBodies move into soft-bodies causing inter-penetration. */
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetSkipRelativeVelocityCheckDuringCollision(bool Skip);

	UFUNCTION(BlueprintPure, Category = "VDynamics")
	FVDCollisionSettings GetCollisionSettings() const;
	void SetCollisionSettings(const FVDCollisionSettings& Settings);

	//UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetCollisionChannelAndResponses(enum ECollisionChannel Channel, struct FCollisionResponse Responses);
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetCollisionChannel(enum ECollisionChannel Channel);

	/** Set this Simulation Objects Object Type. (Pawn, Destructible, etc...) */
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetObjectType(enum ECollisionChannel ObjectType);

	/** Set Collision Response to specified Object Type */
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetCollisionResponseToObjectType(enum ECollisionChannel ObjectType, enum ECollisionResponse Response);

	vicoDynamics::FSimulatedObject* GetUnderlyingSimulatedObject() { return SimulatedObject; }
	
protected:
	// Creates the SimulatedObject instance if not already created
	void CheckInit();

protected:
	struct FVDCollisionSettings CollisionSettings;

private:
	float GravityScale;
	float MinConstraintDistance;

private:
	vicoDynamics::FSimulatedObject* SimulatedObject;

	friend class FVDCollisionQueryInterface;
};