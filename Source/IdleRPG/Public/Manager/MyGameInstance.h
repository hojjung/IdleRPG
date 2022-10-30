// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IdleRPG.h"
#include "LevelManager.h"
#include "Engine/GameInstance.h"
#include "GameMode/IdleRPGGameModeBase.h"
#include "GAS/GAS.h"
#include "Manager/GoldManager.h"
#include "Monsters/SpawnManager.h"
#include "Pet/PetManager.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerPawn.h"
#include "Player/Avatar/AvatarManager.h"
#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"
#include "MyGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	Default,
	PVP,
	BoneDragon,
	Reaper,
	ChickenRun,
	Story,
	Length
};
UCLASS()
class IDLERPG_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	static UMyGameInstance* Get;

	FVoidvoidMulti m_MapChanged;
private:
	TWeakObjectPtr<AMyPlayerPawn> m_Player;
	
	TWeakObjectPtr<AMyPlayerController> m_PlayerCon;

	TArray<FStageRow*> m_AryStage;

	int m_nStageLevel;
	
public:
	TSharedPtr<GAS> m_PlayerGas;
	
	TSharedPtr<GoldManager> m_GoldManager;
	
	TSharedPtr<PetManager> m_PetManager;
	
	TSharedPtr<AvatarManager> m_AvatarManager;
	
	TSharedPtr<LevelManager> m_LevelManager;
	
	
protected:
	virtual void BeginDestroy() override;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	
	void LoadMap(const FName& levelName, FVoidvoid onDead);

	void OnLevelMoveFadeEnd();
	
public:
	virtual void Init() override;

	void SetPlayerPawn(AMyPlayerPawn* p);

	AMyPlayerPawn* GetPlayerPawn();

	void SetPlayerCon(AMyPlayerController* p);

	AMyPlayerController* GetPlayerCon();

	AIdleRPGGameModeBase* GetGameMode();

public:
	FORCEINLINE const TArray<FStageRow*>& GetDefaultStageRows() const
	{
		return m_AryStage;
	}
	
	template <class T>
	void GetNearNpcs(const AActor* caller, TArray<T*>& outAry, float range)
	{
		GetGameMode()->GetNearNpcs<T>(caller, outAry, range);
	}
	
	FText GetStageName();

	int GetStageLevel();
	
	void OnMonsterDead(AMonsterPawn* target);

	void OnMonsterAnimEnd(AMonsterPawn* target);
	
	void OnPlayerDead(AMyPlayerPawn* target);

	void OnPlayerAnimEnd(AMyPlayerPawn* target);
	
public:
	void StartGameMode(EGameMode m, int level, FVoidvoid onLevelChanged);
};


