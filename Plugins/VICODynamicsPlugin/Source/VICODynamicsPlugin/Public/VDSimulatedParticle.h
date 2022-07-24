// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "../VICODynamics/Include/DataContainers.h"
#include "VDSimulation.h"
#include "Components/PrimitiveComponent.h"
#include "VDSimulatedParticle.generated.h"

namespace vicoDynamics
{
	struct FSimulatedParticle;
}

/**
* Particle attachment info
*/
USTRUCT(BlueprintType, Category = "VDynamics")
struct VICODYNAMICSPLUGIN_API FVDParticleAttachment
{
	GENERATED_USTRUCT_BODY()

	FVDParticleAttachment();

	FORCEINLINE FVector CalculateAttachmentWorldLocation(const UPrimitiveComponent* AttachedTo) const
	{
		check(AttachedTo);

// 		if (auto VDComp = Cast<UBaseVDComponent>(AttachedTo) && AttachingToAnotherParticle)
// 			return 
// 		else
			return Socket != NAME_None ? AttachedTo->GetSocketTransform(Socket).TransformPosition(Location) : AttachedTo->GetComponentTransform().TransformPosition(Location);
	}

	FORCEINLINE FVector CalculateAttachmentWorldLocationIgnoreScale(const UPrimitiveComponent* AttachedTo) const
	{
		check(AttachedTo);

		// 		if (auto VDComp = Cast<UBaseVDComponent>(AttachedTo) && AttachingToAnotherParticle)
		// 			return 
		// 		else
		return Socket != NAME_None ? AttachedTo->GetSocketTransform(Socket).TransformPositionNoScale(Location) : AttachedTo->GetComponentTransform().TransformPositionNoScale(Location);
	}

	FORCEINLINE bool IsAttachmentValid() const
	{
		return (ComponentRef.OverrideComponent != nullptr && !ComponentRef.OverrideComponent.IsStale()) ||
			   ComponentRef.OtherActor != nullptr ||
			   ComponentRef.ComponentProperty != NAME_None;
	}

	static FORCEINLINE FName FindComponentPropertyName(const USceneComponent* Component)
	{
		const auto OwnerActor = Component->GetOwner();
		for (TFieldIterator<UObjectProperty> PropIter(OwnerActor->GetClass()); PropIter; ++PropIter)
		{
			if (PropIter->GetPropertyValue(OwnerActor) == Component)
			{
				return PropIter->GetFName();
			}
		}

		return NAME_None;
	}

	/**
	 * Particle to Attach, mainly for in-editor use to identify the particle to attach. Will be improved soon!
	 * Specifying -1 will indicate the end (last) particle.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment", meta = (ClampMin = "-1", UIMin = "-1"))
	int32 ParticleIndex;

	/**
	 * If true, this attachment creates a constraint between two Particles.
	 * ComponentRef must reference a VD*Component and Socket must be a valid Particle index in that Component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	bool AttachingToAnotherParticle;

	/**
	 * If true and the Attached Component simulates physics, tension forces will be simulated between them.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment", meta = (EditCondition = "!AttachingToAnotherParticle"))
	bool SimulateTensionForces;

	/**
	 * Attachment Location. If AttachingToAnotherParticle is true, this is the offset from the Particle.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment")
	FVector Location;

	/**
	 * If true, Location is relative to the ComponentRef. If ComponentRef is null, Location is relative to the Component itself.
	 * If AttachingToAnotherParticle is true, this is always true.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment", meta = (EditCondition = "!AttachingToAnotherParticle"))
	bool IsRelative;

	/**
	 * Component Attached to. If AttachingToAnotherParticle is true, this must be a UBaseVDComponent.
	 */
	UPROPERTY(EditAnywhere, Category = "Attachment", meta = (EditCondition = "IsRelative"))
	FComponentReference ComponentRef;

	/**
	 * Component Socket to Attach to. Location becomes relative to the Sockets transform. If AttachingToAnotherParticle is true, this is the index of the Particle to attach to [0-n].
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment", meta = (EditCondition = "IsRelative"))
	FName Socket;

#if WITH_EDITORONLY_DATA
	/**
	 * If true, highlights this attachment in the world
	 */
	UPROPERTY(EditAnywhere, Category = "Attachment")
	bool Highlight;
#endif

	int32 ShapeIndex;
	float OldMass;

	bool operator==(const FVDParticleAttachment& Other) const
	{
		return ParticleIndex == Other.ParticleIndex && 
			   SimulateTensionForces == Other.SimulateTensionForces &&
			   Location == Other.Location && 
			   IsRelative == Other.IsRelative && 
			   ComponentRef.GetComponent(nullptr) == Other.ComponentRef.GetComponent(nullptr) && 
			   Socket == Other.Socket;
	}

	bool operator!=(const FVDParticleAttachment& Other) const
	{
		return !(*this == Other);
	}

	bool operator<(const FVDParticleAttachment& Other) const
	{
		return ParticleIndex < Other.ParticleIndex;
	}
};

UCLASS(BlueprintType, ClassGroup = VDynamics)
class VICODYNAMICSPLUGIN_API UVDSimulatedParticle : public UObject
{
public:
	GENERATED_BODY()

	UVDSimulatedParticle();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	bool GetIsFree() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetIsFree(bool IsFree);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	float GetRadius() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetRadius(float NewRadius);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	FVector GetLocation() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetLocation(const FVector& NewLocation);

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	FVector GetProjectedLocation() const;
	//UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetProjectedLocation(const FVector& NewProjectedLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	FVector GetVelocity() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetVelocity(const FVector& NewVelocity);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	float GetMass() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetMass(float NewMass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	float GetDragCoefficient() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetDragCoefficient(float NewDragCoefficient);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	float GetFrictionCoefficient() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetFrictionCoefficient(float NewFrictionCoefficient);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	float GetRestitutionCoefficient() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetRestitutionCoefficient(float NewRestitutionCoefficient);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	bool GetIsWindOccluded() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetIsWindOccluded(bool IsOccluded);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	bool GetIsRigidBodyCollisionEnabled() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetIsRigidBodyCollisionEnabled(bool IsEnabled);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VDynamics")
	bool GetIsParticleCollisionEnabled() const;
	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void SetIsParticleCollisionEnabled(bool IsEnabled);

	UFUNCTION(BlueprintCallable, Category = "VDynamics")
	void AddForce(const FVector& Force);

	vicoDynamics::FSimulatedParticle* GetUnderlyingParticle() const;
	void SetUnderlyingParticle(vicoDynamics::FSimulatedParticle* Particle);
	
	/**
	 * Attach this Particle to a Location. If IsRelative is true, Location is assumed to be relative to the owning Component
	 */
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	bool AttachToLocation(FVector Location, bool IsRelative = false);

	/**
	 * Attach this Particle to an Actor, optionally at the Socket. Location is assumed to be relative to the Actor or Socket
	 */
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	bool AttachToActor(AActor* Actor, FVector Location, bool SimulateTension = false, FName Socket = NAME_None);

	/**
	 * Attach this Particle to a Component, optionally at the Socket. Location is assumed to be relative to the Component or Socket
	 */
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	bool AttachToComponent(UPrimitiveComponent* Component, FVector Location, bool SimulateTension = false, FName Socket = NAME_None);

	/**
	 * Detach this Particle
	 */
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void Detach();

	/**
	 * Sets Attachment to the current attachment, if one exists. Returns true if Attachment is valid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attachment")
	bool GetAttachment(FVDParticleAttachment& OutAttachment);

	/**
	 * Cope the passed in Attachment info to this Particle and use it.
	 */
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void CopyAttachment(const FVDParticleAttachment& Attachment, bool UseCurrentMass = true);

	void ProcessAttachment(UPrimitiveComponent* Owner);

	void ToggleSimulation(bool ShouldSimulate);

public:
	/**
	 * If true, this particle is attached and Attachment is valid
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
	bool IsAttached;

	/**
	 *  Valid only if IsAttached is true
	 */
	UPROPERTY()
	FVDParticleAttachment Attachment;

private:
	vicoDynamics::FSimulatedParticle* SimulatedParticle;
	TUniquePtr<TWeakObjectPtr<UVDSimulatedParticle>> SelfPtr;

	bool TensionAttachmentPending;

	// Used in TogleSimulation()
	bool CachedIsFree;
};

USTRUCT(BlueprintType)
struct VICODYNAMICSPLUGIN_API FVDParticleInfo
{
	GENERATED_USTRUCT_BODY();

	FVDParticleInfo() : IsRigidBodyCollisionEnabled(true), IsParticleCollisionEnabled(true)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VDynamics")
	uint32 IsFree : 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VDynamics")
	uint32 IsRigidBodyCollisionEnabled : 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VDynamics")
	uint32 IsParticleCollisionEnabled : 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VDynamics")
	float Radius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VDynamics")
	FVector Location;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VDynamics")
	float Mass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VDynamics")
	float DragCoefficient;
};

UVDSimulatedParticle* VDParticleToUVDParticle(const vicoDynamics::FSimulatedParticle* Particle);