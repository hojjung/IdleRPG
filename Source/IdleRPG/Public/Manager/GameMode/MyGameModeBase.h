// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IdleRPG.h"
#include "Monsters/MonsterPawn.h"
#include "Player/CombatPawn.h"


class SpawnManager;
class AMyPlayerPawn;

class IDLERPG_API MyGameModeBase
{
public:
	MyGameModeBase();
	
	virtual ~MyGameModeBase();

	MyGameModeBase(const MyGameModeBase&) =delete;
	
	MyGameModeBase& operator=(const MyGameModeBase&) =delete;

	FVoidvoidMulti m_LevelChanged;

protected:
	int m_nLevel;

	BigInt m_MobDmg;

	BigInt m_MobHp;

	TSharedPtr<SpawnManager> m_SpawnManager;
	
public:
	virtual void SetLevel(int l);
	
	virtual FText GetStageName();
	
	virtual void Update(float delta_time);
	
	FORCEINLINE BigInt GetHp() { return m_MobHp;}

	FORCEINLINE BigInt GetDmg() { return m_MobDmg;}
	
	virtual void OnMonsterDead(AMonsterPawn* target) {}
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) {}

	virtual void OnPlayerDead(AMyPlayerPawn* target) {}

	FORCEINLINE int GetLevel() {return m_nLevel;}

	template <class T>
	void GetNearNpcs(const AActor* caller, TArray<T*>& outAry, float range)
	{
		m_SpawnManager->GetNearNpcs<T>(caller, outAry, range);
	}
};
