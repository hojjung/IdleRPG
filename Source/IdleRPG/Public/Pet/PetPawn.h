// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Player/MyPlayerPawn.h"
#include "PetPawn.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API APetPawn : public AMyBasePawn
{
	GENERATED_BODY()
	
public:
	APetPawn(const FObjectInitializer& objInit);

protected:
	TWeakObjectPtr<AMyPlayerPawn> m_PlayerPawn;

	float m_fInitZ;

	float m_fMaxZ;

	float m_fMinZ;

	float m_fTimer;

	bool m_bIsMax;

	float m_fFloatingTime;

	float m_fFloatingZ;
public:
	void SetPetEntity(const UUnitAsset* pet_row);

protected:
	virtual void Tick(float DeltaSeconds) override;

	void AnimateFly(float DeltaSeconds);
};
