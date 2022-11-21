// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IdleRPG.h"
#include "MyGameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "GAS/BigInt/BigIntLib.h"
#include "Monsters/SpawnManager.h"
#include "Player/MyPlayerPawn.h"
#include "Widgets/GameLevel/WidgetScreenFadeCanvas.h"
#include "IdleRPGGameModeBase.generated.h"

class UWidgetMainCanvas;
/**
 * 
 */
UCLASS()
class IDLERPG_API AIdleRPGGameModeBase : public AMyGameModeBase
{
	GENERATED_BODY()

public:
	AIdleRPGGameModeBase();

protected:
	TSubclassOf<UWidgetMainCanvas> m_ClassCanvas;
	UPROPERTY()
	UWidgetMainCanvas* m_Canvas;
	TSubclassOf<UWidgetScreenFadeCanvas> m_ClassCanvasFade;
	UPROPERTY()
	UWidgetScreenFadeCanvas* m_CanvasFade;
protected:
	virtual void StartPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
public:
	void SetFade(FVoidvoid onEnd = FVoidvoid());

	void SetHideFade();

	void OpenDeadAlertWidget();

};
