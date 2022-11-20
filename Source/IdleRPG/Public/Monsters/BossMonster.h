// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"

/**
 * 
 */
class IDLERPG_API BossMonster : public Monster
{
public:
	BossMonster(AMonsterPawn* pawn);
	
	virtual ~BossMonster() override;

protected:
	TWeakObjectPtr<AMonsterPawn> m_Pawn;

	TSharedPtr<GAS> m_Gas;

protected:
	enum EFSM
	{
		Idle,
		Chase,
		Combat,
		Length
	};

protected:
	EFSM m_CurrentState;

	FVector m_StartPoint;

	float m_fIdleTimer;

	float m_fChaseFindTimer;

	typedef void (BossMonster::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

	float m_fDeltaTime;

	float m_fAlertTimer;

protected:
	void CheckSetState();
	
	void OnIdle();

	void OnChase();
	
	void OnCombat();

	bool CheckTargetRange(float rangeSqr);

	bool CheckAngle(float angleEuler);
public:
	void SetIdle();

	void ResetStartPosition(FVector loc);
	
public:
	void Update(float delta);
	
	AMonsterPawn* GetMonsterPawn();
};
