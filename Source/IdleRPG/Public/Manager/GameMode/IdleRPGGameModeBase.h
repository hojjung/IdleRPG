// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IdleRPG.h"
#include "GameFramework/GameModeBase.h"
#include "GAS/BigInt/BigIntLib.h"
#include "Monsters/SpawnManager.h"
#include "Player/MyPlayerPawn.h"
#include "IdleRPGGameModeBase.generated.h"

class UWidgetMainCanvas;
/**
 * 
 */
UCLASS()
class IDLERPG_API AIdleRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIdleRPGGameModeBase();

	FVoidvoidMulti m_LevelChanged;
protected:
	TSubclassOf<UWidgetMainCanvas> m_ClassCanvas;
	UPROPERTY()
	UWidgetMainCanvas* m_Canvas;


	int m_nLevel;

	BigInt m_MobDmg;

	BigInt m_MobHp;

	TSharedPtr<SpawnManager> m_SpawnManager;

protected:
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	virtual void SetLevel(int l);
	
	virtual FText GetStageName();
	
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
