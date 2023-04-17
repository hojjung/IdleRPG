// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Widgets/GameLevel/MainMenu/WidgetCurrency.h"
#include "Widgets/GameLevel/MainMenu/Avatar/WidgetAvatarEle.h"
#include "WidgetHUDPlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetHUDPlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetAvatarEle* m_Avatar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgClassIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_WrapboxMenu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMenu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMail;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRank;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPoweroff;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOption;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_ExpBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextExp;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetCurrency* m_Gold;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetCurrency* m_Dia;
	
	
	
protected:
	void BindGold();

	void UpdateGold();
	
	void BindAvatar();
	virtual void NativeOnInitialized() override;

	void SetData(const FName& KeyEquip, const FAvatarRow& row);
	
public:
	UFUNCTION()
	void ToggleMenu();
};
