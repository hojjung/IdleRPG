// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyNavMovement.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UMyNavMovement : public UPawnMovementComponent
{
	GENERATED_BODY()
	

public:
	UMyNavMovement(const FObjectInitializer& obj);

protected:
	TWeakObjectPtr<AMyBasePawn> m_Owner;
	
	FVector m_ImpactVector;

	FVector m_Delta;

	float m_fSpeed;

	float m_fMultiple;
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetMaxSpeed(float spd);

	void SetSpeedMultiple(float m);
	
	void SetImpact(FVector v);

	virtual void SetActive(bool bNewActive, bool bReset=false) override;

	virtual float GetMaxSpeed() const override;

protected:
	virtual void BeginPlay() override;

	void TickRotate(float deltaTime);

	FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const;

	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice=0.f, const FVector& MoveDelta = FVector::ZeroVector) override;

	bool CanStepUp(const FHitResult& Hit) const;

	bool IsNavBound(FVector delta);

	virtual FVector GetDelta(float delta, const FVector& inputDelta);
};

