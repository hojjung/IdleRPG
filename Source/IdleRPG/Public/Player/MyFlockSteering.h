// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Components/MyNavMovement.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Components/ActorComponent.h"
#include "Monsters/MonsterPawn.h"
#include "MyFlockSteering.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IDLERPG_API UMyFlockSteering  : public UMyNavMovement
{
	GENERATED_BODY()

protected:
	TArray<AMonsterPawn*> m_NearMobs;
	
protected:
	virtual void BeginPlay() override;
	
	FVector GetFlockDir();

	virtual void ApplyControlInputToVelocity(float DeltaTime) override;

	virtual void NotifyBumpedPawn(APawn* BumpedPawn) override;	
};
