// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "CombatPawn.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API ACombatPawn : public AMyBasePawn
{
	GENERATED_BODY()

public:
	ACombatPawn(const FObjectInitializer& objInit);
	
protected:
	TWeakObjectPtr<ACombatPawn> m_Target;
	
	bool m_bIsRotateable;

	float m_fAttackCD;

	float m_fAtkRange;

	float m_fAtkRangeSqr;

protected:
	float PlayBaseAttackAnim();

	void SetAtkRange(float v);

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnTickAlive(float DeltaSeconds);
	
public:
	void SetFocusedTarget(ACombatPawn* pawn);
	
	ACombatPawn* GetFocusedTarget();
	
	void HomingRotateToTarget(float speedTime = 5.0f);
	
	float TryAttack(float playRate = 1.f);

	virtual bool IsAlive() {return true;};
	
	virtual bool IsRangeMode() {return true;};

	float GetAttackRange();

	float GetAttackRangeSqr();

	EPathFollowingRequestResult::Type ChaseTarget();
};
