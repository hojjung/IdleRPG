// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/Upgrade/Data/LevelUpData.h"
#include "Player/Upgrade/Data/UpgradeData.h"
#include "WidgetUpgradeEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetUpgradeEle : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextMaxLevel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;

	const FUpgradeDataTableRow* m_UpgradeRow;

	const FLevelUpDataTableRow* m_LevelUpRow;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void SetUpgradeData(const FUpgradeDataTableRow& row);

	void SetUpgradeData(const FLevelUpDataTableRow& row);

	void SetLevel(int lv);
};
