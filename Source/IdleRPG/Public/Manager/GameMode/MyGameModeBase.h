// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CombatPawn.h"


class IDLERPG_API MyGameModeBase
{
public:
	MyGameModeBase();
	virtual ~MyGameModeBase();

	virtual void OnMonsterDied(const ACombatPawn* pawn){};
};
