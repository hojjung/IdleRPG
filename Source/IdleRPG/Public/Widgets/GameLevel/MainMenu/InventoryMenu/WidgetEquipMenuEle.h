// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Equipment/EquipData.h"
#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"
#include "WidgetEquipMenuEle.generated.h"

/**
 * 작은 무기.갑옷 아이콘
 */
UCLASS()
class IDLERPG_API UWidgetEquipMenuEle : public UWidgetItemEle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextMergeCount;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USizeBox* m_SizeEquip;
	UPROPERTY()
	int m_nLv;
	UPROPERTY()
	int m_nAm;
	
protected:
	void UpdateSpec();

public:
	virtual void SetData(const FName& id, const FEntityDataRow& dataEquip) override;

	void SetLevel(int lv);

	void SetAmount(int am);
};


