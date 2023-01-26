// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/MyGameModeBase.h"
#include "Widgets/WidgetConfirmPanel.h"
#include "Widgets/WidgetLoginCanvas.h"
#include "LoginGameMode.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API ALoginGameMode : public AMyGameModeBase
{
	GENERATED_BODY()

public:
	ALoginGameMode();

protected:
	TSubclassOf<UWidgetLoginCanvas> m_ClassWidget;
	UPROPERTY()
	UWidgetLoginCanvas* m_Canvas;
	
protected:
	virtual void BeginPlay() override;

public:
	void PrintInfoText(FText str, FLinearColor color);
	
	void SetConfirmPanel(const FText& txt, UWidgetConfirmPanel::FOnClick onCancel, UWidgetConfirmPanel::FOnClick onConfirm);
};
