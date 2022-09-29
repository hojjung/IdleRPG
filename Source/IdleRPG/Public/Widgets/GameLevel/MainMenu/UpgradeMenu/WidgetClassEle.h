// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Player/Upgrade/Data/ClassUpData.h"
#include "WidgetClassEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetClassEle : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UOverlay* m_Complete;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc; 
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextStat;

public:
	void SetClassData(const FClassUpDataTableRow& row);
};
