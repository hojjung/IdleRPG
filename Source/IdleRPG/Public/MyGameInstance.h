// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Manager/GameMode/MyGameModeBase.h"
#include "Monsters/SpawnManager.h"
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
	static UMyGameInstance* Get;

public:
	TSharedPtr<AvatarManager> m_AvatarManager;
	
	TSharedPtr<SpawnManager> m_SpawnManager;

	TSharedPtr<MyGameModeBase> m_GameMode;
	
	TWeakObjectPtr<AMyPlayerPawn> m_Player;
	
	TWeakObjectPtr<AMyPlayerController> m_PlayerCon;
	
protected:
	virtual void BeginDestroy() override;

public:
	virtual void Init() override;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	
	void Tick(float deltaTime);

	void StartGameMode(EGameMode mode, int level);
};


