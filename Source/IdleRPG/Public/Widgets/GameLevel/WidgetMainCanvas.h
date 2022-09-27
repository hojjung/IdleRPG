// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenu/WidgetAvatarPanel.h"
#include "MainMenu/WidgetMenuBtn.h"
#include "WidgetMainCanvas.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMainCanvas : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetAvatarPanel* m_AvatarPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnSkill;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnInventory;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnAvatar;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnPet;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnMap;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnRelic;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnShop;
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClickAvatarMenu();
};
