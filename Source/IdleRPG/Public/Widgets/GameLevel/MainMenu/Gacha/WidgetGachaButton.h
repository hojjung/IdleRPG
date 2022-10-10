// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "WidgetGachaButton.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetGachaButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextSummon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextCost;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnSummon;
public:
	void Init(const FText& text, int cost, FOnButtonClickedEvent onClick);
};
UCLASS()
class IDLERPG_API UWidgetGachaButtonPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetGachaButton* m_Summon1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetGachaButton* m_Summon2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetGachaButton* m_Summon3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnShowPercentInfo;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* m_BarExp;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_Icon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_Level;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextExp;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UHorizontalBox* m_Btn;
	
public:
	void Init(const FText& text, int costPerOne);

	void HideBottom();

	void UpdateLevel(int level, int cExp, int mExp);

	void SetIcon(UTexture2D* t);

	UFUNCTION()
	void OnSummon1();
	UFUNCTION()
	void OnSummon11();
	UFUNCTION()
	void OnSummon34();
	UFUNCTION()
	void OnClickPercentInfo();
};
