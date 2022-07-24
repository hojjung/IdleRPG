// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDParticleSpringConstraint.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "SimulatedParticle.h"
#include "ParticleConstraints.h"

UVDParticleSpringConstraint::UVDParticleSpringConstraint()
{
	ParticleConstraint = nullptr;
	SelfPtr.Reset(new TWeakObjectPtr<UVDParticleSpringConstraint>(this));
}

UVDSimulatedParticle* UVDParticleSpringConstraint::GetParticle1() const
{
	check(ParticleConstraint);

	return VDParticleToUVDParticle(ParticleConstraint->GetParticle1());
}

UVDSimulatedParticle* UVDParticleSpringConstraint::GetParticle2() const
{
	check(ParticleConstraint);

	return VDParticleToUVDParticle(ParticleConstraint->GetParticle2());
}

float UVDParticleSpringConstraint::GetRestDistance() const
{
	check(ParticleConstraint);

	return ParticleConstraint->GetRestDistance();
}

void UVDParticleSpringConstraint::SetRestDistance(const float& NewRestDistance)
{
	check(ParticleConstraint);

	ParticleConstraint->SetRestDistance(NewRestDistance);
}

float UVDParticleSpringConstraint::GetStifffness() const
{
	check(ParticleConstraint);

	return ParticleConstraint->GetStiffness();
}

void UVDParticleSpringConstraint::SetStiffness(const float& NewStiffness)
{
	check(ParticleConstraint);

	ParticleConstraint->SetStiffness(NewStiffness);
}

bool UVDParticleSpringConstraint::IsConstraining(const UVDSimulatedParticle* Particle1, const UVDSimulatedParticle* Particle2) const
{
	return (ParticleConstraint->GetParticle1() == Particle1->GetUnderlyingParticle() && ParticleConstraint->GetParticle2() == Particle2->GetUnderlyingParticle()) ||
		   (ParticleConstraint->GetParticle1() == Particle2->GetUnderlyingParticle() && ParticleConstraint->GetParticle2() == Particle1->GetUnderlyingParticle());
}

vicoDynamics::FParticleSpringConstraint* UVDParticleSpringConstraint::GetUnderlyingConstraint() const
{
	return ParticleConstraint;
}

void UVDParticleSpringConstraint::SetUnderlyingConstraint(vicoDynamics::FParticleSpringConstraint* Constraint)
{
	check(Constraint);

	ParticleConstraint = Constraint;
	ParticleConstraint->UserData = SelfPtr.Get();
}
