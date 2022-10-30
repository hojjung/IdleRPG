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

protected:
	TSubclassOf<UWidgetMainCanvas> m_ClassCanvas;
	UPROPERTY()
	UWidgetMainCanvas* m_Canvas;

protected:
	virtual void StartPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
public:
	void SetFade(FVoidvoid onEnd = FVoidvoid());

	void SetHideFade();

	void OpenDeadAlertWidget();	
};
