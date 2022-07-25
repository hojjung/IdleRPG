// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "QuadTree.h"
#include "StageTable.h"

/**
 * 
 */
class IDLERPG_API SpawnManager
{
public:
	SpawnManager();
	~SpawnManager();
	

protected:
	TArray<FStageRow*> m_AryStage;
	
	TArray<TSharedPtr<Monster>> m_AryMonsters;

	TSharedPtr<QuadTree> m_QuadTree;

public:
	void Update(float delta);

	void SpawnUnits(UObject* world, int stageLevel, int cnt);
	
	void GetNearNpcs(const TWeakObjectPtr<AMyBasePawn>& base, TArray<AMonsterPawn*>& outAry, float range);
};
