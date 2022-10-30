// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "QuadTree.h"
#include "Player/CombatPawn.h"

class MyGameModeBase;
class UNavigationSystemV1;
/**
 * 
 */
class IDLERPG_API SpawnManager
{
public:
	SpawnManager();
	
	~SpawnManager();
	
private:
	TArray<TSharedPtr<Monster>> m_AryMonsters;

	TSharedPtr<QuadTree> m_QuadTree;

	TWeakObjectPtr<const UNavigationSystemV1> m_Nav;

	FBox m_NavBox;

	BigInt m_Hp;

	BigInt m_Dmg;
	
private:
	void OnMonsterLoaded(const FPrimaryAssetId id, const UObject* world, FVector loc, FRotator rot);

public:
	void SetBigIntStagMob(BigInt hp, BigInt dmg);
	
	const FPrimaryAssetId& GetRandomMonsterID(const TArray<FPrimaryAssetId>& z);
	
	void Update(float delta);

	void SpawnUnits(const FPrimaryAssetId& id);
	
	template <class T>
	void GetNearNpcs(const AActor* caller, TArray<T*>& outAry, float range)
	{
		if(!m_QuadTree)
		{
			return;
		}
		m_QuadTree->TraceObjectInRange<T>(caller,range, outAry);
	}
};
