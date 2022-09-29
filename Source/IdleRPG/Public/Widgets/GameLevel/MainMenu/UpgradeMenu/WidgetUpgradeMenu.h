// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetClassPanel.h"
#include "WidgetEmblemPanel.h"
#include "WidgetLevelStatPanel.h"
#include "WidgetUpgradePanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetUpgradeMenu.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetUpgradeMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetUpgradePanel* m_UpgradePanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetLevelStatPanel* m_LevelStatPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetClassPanel* m_ClassPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEmblemPanel* m_EmblemPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnLevelStat;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClass;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnEmblem;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void OnOpen();
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OpenUpgrade();
	UFUNCTION()
	void OpenLevelStat();
	UFUNCTION()
	void OpenClass();
	UFUNCTION()
	void OpenEmblem();
};


