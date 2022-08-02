// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AMonsterPawn;
class AMyPlayerPawn;
/**
 * 
 */
class IDLERPG_API PlayerSensor
{
public:
	PlayerSensor(AMyPlayerPawn* pl);
	
	~PlayerSensor();

protected:
	TWeakObjectPtr<AMyPlayerPawn> m_Pl;

	TArray<AMonsterPawn*> m_AryMobs;

	FTimerHandle TimerHandle_OnTimer;

	float m_SensingTimer;

protected:
	AMonsterPawn* GetNearTarget(float SearchRange);

public:
	void Update(float delta);
};
