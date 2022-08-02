// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AMyPlayerPawn;
/**
 * 
 */
class IDLERPG_API PlayerFSM
{
public:
	PlayerFSM(AMyPlayerPawn* pl);
	
	~PlayerFSM();

	enum EFSM
	{
		Idle,
		Chase,
		Combat,
		Length
	};
	
protected:
	TWeakObjectPtr<AMyPlayerPawn> m_Pl;

	EFSM m_CurrentState;

	FVector m_StartPoint;

	float m_fDeltaTime;

	typedef void (PlayerFSM::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];
	
protected:
	void CheckSetState();
	
	void OnIdle();
	
	void OnChase();
	
	void OnCombat();
	
	void ResetStartPosition(FVector loc);

	bool CheckTargetRange(float rangeSqr);

	bool CheckAngle(float angleEuler);
	
public:
	void Update(float delta);
};
