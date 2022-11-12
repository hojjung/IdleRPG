// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExpManager.h"
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
#include "StageMode/StageModeBase.h"
#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"
#include "MyGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	Default,
	PVP,
	Mine,
	Elf,
	Japan,
	BoneDragon,
	Reaper,
	Castle,
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

	int m_nStageLevel;

	EGameMode m_GameMode;
	
public:
	TSharedPtr<GAS> m_PlayerGas;
	
	TSharedPtr<GoldManager> m_GoldManager;

	TSharedPtr<ExpManager> m_ExpManager;
	
	TSharedPtr<PetManager> m_PetManager;
	
	TSharedPtr<AvatarManager> m_AvatarManager;
	
	TSharedPtr<LevelManager> m_LevelManager;

	UPROPERTY()
	UStageModeBase* m_StageMode;

	bool m_bIsPlayerDead;
	
protected:
	virtual void BeginDestroy() override;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;

	void LoadMap(const FName& levelName);

	void OnLevelMoveFadeEnd();
public:
	virtual void Init() override;

	void OnGameModeStart();
	
	void StartGameMode(EGameMode m, int level);
	
	void Tick(float d);
	
	void SetPlayerPawn(AMyPlayerPawn* p);

	AMyPlayerPawn* GetPlayerPawn();

	void SetPlayerCon(AMyPlayerController* p);

	AMyPlayerController* GetPlayerCon();

	AIdleRPGGameModeBase* GetGameMode();

	UStageModeBase * GetStageMode();

	void TryOpenDeadAlert();
};


