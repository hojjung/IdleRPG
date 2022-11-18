// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetMapSelect.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ConquerMap/ContentData.h"
#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"
#include "WidgetMapInfo.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMapInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnEnter; 
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextKeyCount;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetItemEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* m_ScrollPrize;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextDailyMax;

	const FContentDataRow* m_ContentData;

	int m_nLv;
	
protected:
	virtual void NativeOnInitialized() override;
	
public:
	
	void SetInfo(const FContentDataRow* content_data_row, int lv);

	UFUNCTION()
	void OnClick();
};
