// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"

class AMonsterPawn;
/**
 *  쿼드트리?
 */
class IDLERPG_API Monster
{
public:
	Monster(AMonsterPawn* pawn, UUnitEntityAsset* asset);
	
	~Monster();

protected:
	TWeakObjectPtr<AMonsterPawn> m_Pawn;

	float m_fMaxHp;
	
	float m_fHp;

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

	float m_fAttackRange; //from startPos

	float m_fAttackRangeSqr; //from startPos

	typedef void (Monster::*FPtrState)(void);

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
	
	float GetHpPercent();

	FORCEINLINE float GetAttackRange()
	{
		return m_fAttackRange;
	}

	FORCEINLINE float GetAttackRangeSqr()
	{
		return m_fAttackRangeSqr;
	}
};
