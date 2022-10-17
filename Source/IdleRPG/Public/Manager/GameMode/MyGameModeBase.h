// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monsters/MonsterPawn.h"
#include "Player/CombatPawn.h"


class IDLERPG_API MyGameModeBase
{
public:
	MyGameModeBase();
	virtual ~MyGameModeBase();
	
	virtual void OnMonsterDead(AMonsterPawn* target) {};
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) {};
};
