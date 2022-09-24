// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Actors/Components/MyNavMovement.h>
#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Monsters/MonsterPawn.h"
#include "MyFlockSteering.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IDLERPG_API UMyFlockSteering  : public UMyNavMovement
{
	GENERATED_BODY()

public:
	UMyFlockSteering(const FObjectInitializer& obj);

protected:
	TArray<AMonsterPawn*> m_NearMobs;

	float m_fRadius;

protected:
	virtual void BeginPlay() override;
	
	virtual void ApplyControlInputToVelocity(float DeltaTime) override;

	FVector GetBoidDelta(FVector inputDelta);
};
