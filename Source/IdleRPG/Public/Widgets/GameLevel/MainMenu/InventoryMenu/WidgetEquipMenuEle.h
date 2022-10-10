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
#include "WidgetEquipMenuEle.generated.h"

/**
 * 작은 무기.갑옷 아이콘
 */
UCLASS()
class IDLERPG_API UWidgetEquipMenuEle : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClick, const FEquipRow&);

protected:
	FOnClick m_OnClick;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgGlow;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgTier;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USizeBox* m_SizeEquip;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextTierLevel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextCount;

	const FEquipRow* m_Row;

	int m_nSortOrder;
	
public:
	void SetEquipData(const FEquipRow& dataEquip, const FOnClick& onClick);

	int GetSortOrder() const;

	void SetEquipSpec(int level, bool isEquip, int amount);

protected:
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
};


