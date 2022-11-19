// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/StageMenu/WidgetHUDStageButton.h"
#include "MainMenu/StageMenu/WidgetStagePanel.h"
#include "WidgetDefaultCanvas.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetDefaultCanvas : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetStagePanel* m_StagePanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetHUDStageButton* m_StageBtn;

protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnOpenStage();
};
