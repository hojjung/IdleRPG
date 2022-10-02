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
class ENTITY_API UMyNavMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()
	

public:
	UMyNavMovement(const FObjectInitializer& obj);

public:
	float m_fSpeedMultiple;
	
protected:
	TWeakObjectPtr<AMyBasePawn> m_Owner;
	
	FVector m_ImpactVector;

	FVector m_Delta;
	
protected:
	void MySnapToNav();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetImpact(FVector v);

	virtual void SetActive(bool bNewActive, bool bReset=false) override;

protected:
	virtual void BeginPlay() override;

	void TickRotate(float deltaTime);

	FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const;

	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice=0.f, const FVector& MoveDelta = FVector::ZeroVector) override;

	bool CanStepUp(const FHitResult& Hit) const;

	bool IsNavBound(FVector delta);
};
