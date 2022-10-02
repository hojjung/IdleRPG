// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Player/CombatPawn.h"
#include "MonsterPawn.generated.h"

class UWidgetPawnInfoComp;
class Monster;
/**
 * 
 */
UCLASS()
class IDLERPG_API AMonsterPawn : public ACombatPawn
{
	GENERATED_BODY()

public:
	AMonsterPawn(const FObjectInitializer& obj);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UWidgetPawnInfoComp* m_PawnInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* m_SoundComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* m_HitParticle;
	
protected:
	TWeakPtr<Monster> m_MonsterInst;

public:
	void SetMonsterInst(const TSharedPtr<Monster>& mob);
	
	float GetHpPercent() const;

	virtual bool IsAlive() override;

	virtual bool UseBoidMove() override;
};
