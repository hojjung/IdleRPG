// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "QuadTree.h"
#include "StageTable.h"
#include "Engine/StreamableManager.h"
#include "Player/CombatPawn.h"

/**
 * 
 */
class IDLERPG_API SpawnManager
{
public:
	SpawnManager(int stageLevel);
	~SpawnManager();
	
private:
	TArray<FStageRow*> m_AryStage;
	
	TArray<TSharedPtr<Monster>> m_AryMonsters;

	TSharedPtr<QuadTree> m_QuadTree;

	int m_nStageLevel;
private:
	const FPrimaryAssetId& GetRandomMonsterID(int stageLevel, const FZone& z);

	const FPrimaryAssetId& GetBossMonster(int stageLevel);

	void OnMonsterLoaded(const FPrimaryAssetId id, const UObject* world, FVector loc, FRotator rot,  ACombatPawn::FOnDied dele);
	
public:
	const FStageRow& GetStage(int stageLevel);
	
	const FZone& GetZone(int stageLevel);
	
	void Update(float delta);

	void SpawnUnits(const UObject* world, int stageLevel, ACombatPawn::FOnDied dele, int cnt = 20);
	
	template <class T>
	void GetNearNpcs(const AActor* caller, TArray<T*>& outAry, float range)
	{
		if(!m_QuadTree)
		{
			return;
		}
		m_QuadTree->TraceObjectInRange<T>(caller,range, outAry);
	}
	
	void Clear();

	int GetStageLevel();
};
