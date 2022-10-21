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

protected:
	int m_nLevel;

	BigInt m_MobDmg;

	BigInt m_MobHp;
	
public:
	virtual void OnMonsterDead(AMonsterPawn* target) {};
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) {};

	virtual void SetLevel(int lv)
	{
		m_nLevel = lv;
	};

	BigInt GetHp() { return m_MobHp;};

	BigInt GetDmg() { return m_MobDmg;};
};
