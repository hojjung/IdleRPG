// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Relic/RelicData.h"
#include "WidgetRelicEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetRelicEle : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClick, const FRelicRow&)

protected:
	FOnClick m_OnClick;

	const FRelicRow* m_Row;

	int m_nSortOrder;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextRelicName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextMaxLevel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextPercent;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextCount;
public:
	void SetRelicData(const FRelicRow& row, const FOnClick& delegate);

	void SetRelicSpec(int currentLevel, int amountHave);

public:
	UFUNCTION()
	void OnClickButton();
	
	int GetSortOrder() const;
};
