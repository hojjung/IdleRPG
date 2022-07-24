// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "VDSimulation.h"
#include "VDSimulatedParticle.h"
#include "VDParticleSpringConstraint.generated.h"

namespace vicoDynamics
{
	struct FSimulatedParticle;
	struct FParticleSpringConstraint;
}

UCLASS(BlueprintType, ClassGroup = VDynamics)
class VICODYNAMICSPLUGIN_API UVDParticleSpringConstraint : public UObject
{
public:
	GENERATED_BODY()

	UVDParticleSpringConstraint();
	
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	UVDSimulatedParticle* GetParticle1() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	UVDSimulatedParticle* GetParticle2() const;

	UFUNCTION(BlueprintPure, Category = "VDynamics")
	float GetRestDistance() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetRestDistance(const float& NewRestDistance);

	UFUNCTION(BlueprintPure, Category = "VDynamics")
	float GetStifffness() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetStiffness(const float& NewStiffness);

	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	bool IsConstraining(const UVDSimulatedParticle* Particle1, const UVDSimulatedParticle* Particle2) const;

	vicoDynamics::FParticleSpringConstraint* GetUnderlyingConstraint() const;
	void SetUnderlyingConstraint(vicoDynamics::FParticleSpringConstraint* Constraint);

private:
	vicoDynamics::FParticleSpringConstraint* ParticleConstraint;
	TUniquePtr<TWeakObjectPtr<UVDParticleSpringConstraint>> SelfPtr;
};