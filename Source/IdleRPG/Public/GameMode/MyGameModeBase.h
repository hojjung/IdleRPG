// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class UWidgetAlertPanel;
/**
 * 
 */
UCLASS()
class IDLERPG_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameModeBase();

protected:
	TSubclassOf<UWidgetAlertPanel> m_ClassCanvasAlert;
	UPROPERTY()
	UWidgetAlertPanel* m_CanvasAlert;
	
public:
	virtual void StartPlay() override;

	void ShowText(const FText& str, FLinearColor color);
};
