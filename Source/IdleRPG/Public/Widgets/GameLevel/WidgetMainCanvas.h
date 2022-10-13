// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/WidgetMenuBtn.h"
#include "MainMenu/Acce/WidgetAcceMenu.h"
#include "MainMenu/Avatar/WidgetAvatarPanel.h"
#include "MainMenu/InventoryMenu/WidgetInvenPanel.h"
#include "MainMenu/Map/WidgetMenuMap.h"
#include "MainMenu/Pet/WidgetPetPanel.h"
#include "MainMenu/Relic/WidgetRelicPanel.h"
#include "MainMenu/Shop/WidgetShopPanel.h"
#include "MainMenu/Skill/WidgetSkillPanel.h"
#include "MainMenu/StageMenu/WidgetHUDStageButton.h"
#include "MainMenu/StageMenu/WidgetStagePanel.h"
#include "MainMenu/UpgradeMenu/WidgetUpgradeMenu.h"
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
	UWidgetUpgradeMenu* m_UpgradePanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetSkillPanel* m_SkillPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetInvenPanel* m_InvenPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetPetPanel* m_PetPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetAvatarPanel* m_AvatarPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuMap* m_MapPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetAcceMenu* m_AccePanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetRelicPanel* m_RelicPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetShopPanel* m_ShopPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetStagePanel* m_StagePanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetHUDStageButton* m_StageBtn;
	//
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnSkill;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnInventory;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnPet;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnAvatar;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnMap;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnAcce;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnRelic;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMenuBtn* m_BtnShop;
	
protected:
	virtual void NativeOnInitialized() override;

	void TryOpen(UWidgetMenuBase* menu);

public:
	UFUNCTION()
	void OnClickAccePanel();
	UFUNCTION()
	void OnClickShopPanel();
	UFUNCTION()
	void OnClickSkillPanel();
	UFUNCTION()
	void OnClickPetMenu();
	UFUNCTION()
	void OnClickAvatarMenu();
	UFUNCTION()
	void OnOpenUpgradePanel();
	UFUNCTION()
	void OnClickInvenPanel();
	UFUNCTION()
	void OnClickRelicPanel();
	UFUNCTION()
	void OnClickMapPanel();
	UFUNCTION()
	void OnOpenStage();
};
