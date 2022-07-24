// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDSimulatedParticle.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "../VICODynamics/Include/SimulatedParticle.h"

#include "BaseVDComponent.h"

UVDSimulatedParticle::UVDSimulatedParticle()
{
	SimulatedParticle = nullptr;
	SelfPtr.Reset(new TWeakObjectPtr<UVDSimulatedParticle>(this));
}

bool UVDSimulatedParticle::GetIsFree() const
{
	check(SimulatedParticle);
	
	return SimulatedParticle->GetIsFree();
}

void UVDSimulatedParticle::SetIsFree(bool IsFree)
{
	check(SimulatedParticle);
	
	SimulatedParticle->SetIsFree(IsFree);
}

float UVDSimulatedParticle::GetRadius() const
{
	check(SimulatedParticle);

	return SimulatedParticle->GetRadius();
}

void UVDSimulatedParticle::SetRadius(float NewRadius)
{
	check(SimulatedParticle);

	SimulatedParticle->SetRadius(NewRadius);
}

FVector UVDSimulatedParticle::GetLocation() const
{
	check(SimulatedParticle);
		
	return VDVectorToUVector(SimulatedParticle->GetLocation());
}

void UVDSimulatedParticle::SetLocation(const FVector& NewLocation)
{
	check(SimulatedParticle);

	SimulatedParticle->SetLocation(UVectorToVDVector(NewLocation));
}

FVector UVDSimulatedParticle::GetProjectedLocation() const
{
	check(SimulatedParticle);

	return VDVectorToUVector(SimulatedParticle->GetProjectedLocation());
}

void UVDSimulatedParticle::SetProjectedLocation(const FVector& NewProjectedLocation)
{
	check(SimulatedParticle);

	SimulatedParticle->SetProjectedLocation(UVectorToVDVector(NewProjectedLocation));
}

FVector UVDSimulatedParticle::GetVelocity() const
{
	check(SimulatedParticle);

	return VDVectorToUVector(SimulatedParticle->GetVelocity());
}

void UVDSimulatedParticle::SetVelocity(const FVector& NewVelocity)
{
	check(SimulatedParticle);

	SimulatedParticle->SetVelocity(UVectorToVDVector(NewVelocity));
}

float UVDSimulatedParticle::GetMass() const
{
	check(SimulatedParticle);

	return SimulatedParticle->GetMass();
}

void UVDSimulatedParticle::SetMass(float NewMass)
{
	check(SimulatedParticle);

	SimulatedParticle->SetMass(NewMass);
}

float UVDSimulatedParticle::GetDragCoefficient() const
{
	check(SimulatedParticle);

	return SimulatedParticle->GetDragCoefficient();
}

void UVDSimulatedParticle::SetDragCoefficient(float NewDragCoefficient)
{
	check(SimulatedParticle);

	SimulatedParticle->SetDragCoefficient(NewDragCoefficient);
}

float UVDSimulatedParticle::GetFrictionCoefficient() const
{
	check(SimulatedParticle);

	return SimulatedParticle->GetFriction();
}

void UVDSimulatedParticle::SetFrictionCoefficient(float NewFrictionCoefficient)
{
	check(SimulatedParticle);

	SimulatedParticle->SetFriction(NewFrictionCoefficient);
}

float UVDSimulatedParticle::GetRestitutionCoefficient() const
{
	check(SimulatedParticle);

	return SimulatedParticle->GetRestitution();
}

void UVDSimulatedParticle::SetRestitutionCoefficient(float NewRestitutionCoefficient)
{
	check(SimulatedParticle);

	SimulatedParticle->SetRestitution(NewRestitutionCoefficient);
}

bool UVDSimulatedParticle::GetIsWindOccluded() const
{
	check(SimulatedParticle);

	return SimulatedParticle->GetIsOccludedFromWind();
}

void UVDSimulatedParticle::SetIsWindOccluded(bool IsOccluded)
{
	check(SimulatedParticle);

	SimulatedParticle->SetIsOccludedFromWind(IsOccluded);
}

bool UVDSimulatedParticle::GetIsRigidBodyCollisionEnabled() const
{
	check(SimulatedParticle);

	return !(SimulatedParticle->GetFlags() & vicoDynamics::EParticleFlag::Collision_DisableRigidBodyCollision);
}

void UVDSimulatedParticle::SetIsRigidBodyCollisionEnabled(bool IsEnabled)
{
	check(SimulatedParticle);

	if (IsEnabled)
		SimulatedParticle->RemoveFlags(vicoDynamics::EParticleFlag::Collision_DisableRigidBodyCollision);
	else
		SimulatedParticle->SetFlags(vicoDynamics::EParticleFlag::Collision_DisableRigidBodyCollision);
}

bool UVDSimulatedParticle::GetIsParticleCollisionEnabled() const
{
	check(SimulatedParticle);

	return !(SimulatedParticle->GetFlags() & vicoDynamics::EParticleFlag::Collision_DisableParticleCollision);
}

void UVDSimulatedParticle::SetIsParticleCollisionEnabled(bool IsEnabled)
{
	check(SimulatedParticle);

	if (IsEnabled)
		SimulatedParticle->RemoveFlags(vicoDynamics::EParticleFlag::Collision_DisableParticleCollision);
	else
		SimulatedParticle->SetFlags(vicoDynamics::EParticleFlag::Collision_DisableParticleCollision);
}

void UVDSimulatedParticle::AddForce(const FVector& Force)
{
	check(SimulatedParticle);

	SimulatedParticle->AddForce(UVectorToVDVector(Force));
}

vicoDynamics::FSimulatedParticle* UVDSimulatedParticle::GetUnderlyingParticle() const
{
	return SimulatedParticle;
}

void UVDSimulatedParticle::SetUnderlyingParticle(vicoDynamics::FSimulatedParticle* Particle)
{
	check(Particle);

	SimulatedParticle = Particle;
	SimulatedParticle->UserData = SelfPtr.Get();
}

bool UVDSimulatedParticle::AttachToLocation(FVector Location, bool IsRelative /*= false*/)
{
	Attachment = FVDParticleAttachment();
	Attachment.Location = Location;
	Attachment.IsRelative = IsRelative;
	Attachment.OldMass = GetMass();

	SetMass(0.f);
	SetIsFree(false);

	IsAttached = true;

	return IsAttached;
}

bool UVDSimulatedParticle::AttachToActor(AActor* Actor, FVector Location, bool SimulateTension /*= false*/, FName Socket /*= NAME_None*/)
{
	if (!Actor) return false;

	Attachment = FVDParticleAttachment();
	Attachment.ComponentRef.OtherActor = Actor;
	Attachment.Location = Location;
	Attachment.Socket = Socket;
	Attachment.IsRelative = true;
	Attachment.OldMass = GetMass();

	if (!SimulateTension)
	{
		SetMass(0.f);
		SetIsFree(false);
	}
	else
	{
		Attachment.SimulateTensionForces = SimulateTension;
		TensionAttachmentPending = true;
	}

	IsAttached = true;

	return IsAttached;
}

bool UVDSimulatedParticle::AttachToComponent(UPrimitiveComponent* Component, FVector Location, bool SimulateTension /*= false*/, FName Socket /*= NAME_None*/)
{
	if (!Component) return false;

	Attachment = FVDParticleAttachment();
	const auto OwnerActor = Component->GetOwner();
	Attachment.ComponentRef.OverrideComponent = Component;
	Attachment.Location = Location;
	Attachment.Socket = Socket;
	Attachment.IsRelative = true;
	Attachment.OldMass = GetMass();

	if (!SimulateTension)
	{
		SetMass(0.f);
		SetIsFree(false);
	}
	else
	{
		Attachment.SimulateTensionForces = SimulateTension;
		TensionAttachmentPending = true;
	}

	IsAttached = true;

	return IsAttached;
}

void UVDSimulatedParticle::Detach()
{
	if (IsAttached)
	{
		SetMass(Attachment.OldMass);
		SetIsFree(true);

		IsAttached = false;
	}
}

bool UVDSimulatedParticle::GetAttachment(FVDParticleAttachment& OutAttachment)
{
	if (IsAttached)
	{
		OutAttachment = Attachment;
		return true;
	}

	return false;
}

void UVDSimulatedParticle::CopyAttachment(const FVDParticleAttachment& AttachmentToCopy, bool UseCurrentMass/* = true*/)
{
	Attachment = FVDParticleAttachment();
	Attachment.ParticleIndex = AttachmentToCopy.ParticleIndex;
	Attachment.ShapeIndex = AttachmentToCopy.ShapeIndex;
	Attachment.AttachingToAnotherParticle = AttachmentToCopy.AttachingToAnotherParticle;
	Attachment.ComponentRef = AttachmentToCopy.ComponentRef;
	Attachment.SimulateTensionForces = AttachmentToCopy.SimulateTensionForces;
	Attachment.Location = AttachmentToCopy.Location;
	Attachment.Socket = AttachmentToCopy.Socket;
	Attachment.IsRelative = AttachmentToCopy.IsRelative;
	if (UseCurrentMass)
		Attachment.OldMass = GetMass();
	else
		Attachment.OldMass = AttachmentToCopy.OldMass;

	if (Attachment.AttachingToAnotherParticle)
	{
		SetIsFree(true);
	}
	else if (!Attachment.SimulateTensionForces)
	{
		SetMass(0.f);
		SetIsFree(false);
	}

	IsAttached = true;
}

void UVDSimulatedParticle::ProcessAttachment(UPrimitiveComponent* Owner)
{
	if (IsAttached && Attachment.AttachingToAnotherParticle)
		return;

	// If we need to simulate tension forces, BaseVDComponent will process this attachment
	if (!IsAttached || Attachment.SimulateTensionForces)
	{
		// Check if we have a pending Tension attachment to create
		if (TensionAttachmentPending && Attachment.SimulateTensionForces)
		{
			auto BaseVDComp = Cast<UBaseVDComponent>(Owner);
			if (BaseVDComp)
			{
				Attachment.ParticleIndex = BaseVDComp->Particles.IndexOfByKey(this);
				BaseVDComp->CreateTensionConstraintsFromAttachment(Attachment);
				TensionAttachmentPending = false;
			}
		}
		return;
	}

	if (Attachment.IsRelative)
	{
		const UPrimitiveComponent* PrimComp = Attachment.IsAttachmentValid() ? Cast<UPrimitiveComponent>(Attachment.ComponentRef.GetComponent(Owner->GetOwner())) : Owner;
		const FVector AttachedPos = Attachment.CalculateAttachmentWorldLocationIgnoreScale(PrimComp ? PrimComp : Owner);
		SetLocation(AttachedPos);
	}
	else
	{
		SetLocation(Attachment.Location);
	}
}

void UVDSimulatedParticle::ToggleSimulation(bool ShouldSimulate)
{
	if (ShouldSimulate)
	{
		SetIsFree(CachedIsFree);
	}
	else
	{
		CachedIsFree = GetIsFree();
		SetIsFree(false);
	}
}

FVDParticleAttachment::FVDParticleAttachment()
{
	ParticleIndex = 0;
	AttachingToAnotherParticle = false;
	SimulateTensionForces = false;
	Location = FVector::ZeroVector;
	ComponentRef.OtherActor = nullptr;
	ComponentRef.OverrideComponent = nullptr;
	Socket = NAME_None;
	IsRelative = true;
}

UVDSimulatedParticle* VDParticleToUVDParticle(const vicoDynamics::FSimulatedParticle* Particle)
{
	if (Particle)
	{
		auto userData = Particle->UserData;
		if (userData)
		{
			auto objPtr = static_cast<TWeakObjectPtr<UVDSimulatedParticle>*>(userData);
			if (objPtr)
			{
				auto particle = objPtr->Get();
				if (particle)
				{
					return particle;
				}
			}
		}
	}

	return nullptr;
}