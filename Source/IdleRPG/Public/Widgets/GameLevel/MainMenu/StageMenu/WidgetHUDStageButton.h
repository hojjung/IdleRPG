// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Manager/MyGameInstance.h"
#include "WidgetHUDStageButton.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetHUDStageButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnOpenStage;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextStageName;

protected:
	virtual void NativeOnInitialized() override;

	void UpdateText(EGameMode mode, int level);
	
public:
	UButton* GetBtn();
};
