// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BigInt/BigIntLib.h"
#include "Monsters/SpawnManager.h"
#include "Player/MyPlayerPawn.h"
#include "UObject/NoExportTypes.h"
#include "StageModeBase.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UStageModeBase : public UObject
{
	GENERATED_BODY()
	
protected:
	int m_nStageLevel;

	BigInt m_MobDmg;

	BigInt m_MobHp;

	BigInt m_Gold;

	BigInt m_Exp;

	TSharedPtr<SpawnManager> m_SpawnManager;

protected:
	virtual void BeginDestroy() override;

	virtual TArray<FPrimaryAssetId> GetStageUnits(int lv);
	
	virtual void OnPreSpawnMobs();

	void SpawnMobs();
	
public:
	void SetLevel(int l);
	
	FText GetStageName();

	virtual FText GetStageName(int level);

public:
	FORCEINLINE int GetLevel()
	{
		return m_nStageLevel;
	}
	FORCEINLINE BigInt GetHp()
	{
		return m_MobHp;
	}
	FORCEINLINE BigInt GetDmg()
	{
		return m_MobDmg;
	}
	FORCEINLINE BigInt GetGold()
	{
		return m_Gold;
	}
	FORCEINLINE BigInt GetExp()
	{
		return m_Exp;
	}
	virtual void OnMonsterDead(AMonsterPawn* target);
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) {}

	virtual void OnPlayerDead(AMyPlayerPawn* target) {}

	virtual void OnPlayerAnimEnd(AMyPlayerPawn* target) {}

	template <class T>
	void GetNearNpcs(const AActor* caller, TArray<T*>& outAry, float range)
	{
		m_SpawnManager->GetNearNpcs<T>(caller, outAry, range);
	}

	virtual void Tick(float d);
};
