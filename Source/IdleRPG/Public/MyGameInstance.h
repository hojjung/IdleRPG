// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GAS/GAS.h"
#include "Manager/GoldManager.h"
#include "Manager/GameMode/MyGameModeBase.h"
#include "Monsters/SpawnManager.h"
#include "Pet/PetManager.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerPawn.h"
#include "Player/Avatar/AvatarManager.h"
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
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMapChange, EGameMode, int);

	FOnMapChange m_OnMapChange;

public:
	static UMyGameInstance* Get;

private:
	TWeakObjectPtr<AMyPlayerPawn> m_Player;
	
	TWeakObjectPtr<AMyPlayerController> m_PlayerCon;
	
public:
	TSharedPtr<GAS> m_PlayerGas;
	
	TSharedPtr<GoldManager> m_GoldManager;
	
	TSharedPtr<PetManager> m_PetManager;
	
	TSharedPtr<AvatarManager> m_AvatarManager;
	
	TSharedPtr<SpawnManager> m_SpawnManager;

	TSharedPtr<MyGameModeBase> m_GameMode;
	
	
protected:
	virtual void BeginDestroy() override;

public:
	virtual void Init() override;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	
	void Tick(float deltaTime);

	void StartGameMode(EGameMode mode, int level);

	FText GetDefaultStageName(int level);

	int GetStageLevel();

	void SetPlayerPawn(AMyPlayerPawn* p);

	AMyPlayerPawn* GetPlayerPawn();

	void SetPlayerCon(AMyPlayerController* p);

	AMyPlayerController* GetPlayerCon();
};


