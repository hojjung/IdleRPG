// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetConsumInven.h"
#include "WidgetEquipMenu.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"
#include "WidgetInvenPanel.generated.h"

/**
 * 554023FF
 *
 * 9C8967FF
 */
UCLASS()
class IDLERPG_API UWidgetInvenPanel : public UWidgetMenuBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetConsumInven* m_ConsumInven;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipMenu* m_EquipWeapon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipMenu* m_EquipHelmet;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipMenu* m_EquipTorso;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipMenu* m_EquipGlove;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipMenu* m_EquipLeg;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipMenu* m_EquipFeet;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnConsumable;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnWeapon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnArmor1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnArmor2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnArmor3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnArmor4;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnArmor5;

	TWeakObjectPtr<UButton> m_CurrentBtn;
protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void OnOpen() override;
	virtual void OnClose() override;

	UFUNCTION()
	void OnClickConsume();
	UFUNCTION()
	void OnClickWeapon();
	UFUNCTION()
	void OnClickArmor1();
	UFUNCTION()
	void OnClickArmor2();
	UFUNCTION()
	void OnClickArmor3();
	UFUNCTION()
	void OnClickArmor4();
	UFUNCTION()
	void OnClickArmor5();
};
