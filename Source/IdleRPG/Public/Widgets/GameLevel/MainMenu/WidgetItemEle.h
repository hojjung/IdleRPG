// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "DataTableRow/EntityData.h"
#include "WidgetItemEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetItemEle : public UUserWidget
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE_TwoParams(FOnClick, const FName&, const FEntityDataRow&);

	FORCEINLINE FOnClick& GetOnClick()
	{
		return m_OnClick; 
	}
protected:
	FOnClick m_OnClick;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (ExposeOnSpawn))
	FVector2D m_Size = FVector2D(150,150);
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USizeBox* m_SizeEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgGlow;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgTier;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextTierLevel;//티어와 타입
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextCount;

protected:
	FName m_ID;
	
	const FEntityDataRow* m_Row;

	int m_nSortOrder;

	bool m_bHasTouch;
	
public:
	virtual void SetDataOnClick( const FName& id, const FEntityDataRow& dataEquip, FOnClick onClick);
	
	virtual void SetData( const FName& id, const FEntityDataRow& dataEquip);

	int GetSortOrder() const;

	void SetTopText(const FString& str, FLinearColor c = FLinearColor::White);

	void SetTopText(const FText& str, FLinearColor c = FLinearColor::White);

	void SetBottomText(const FString& str, FLinearColor c = FLinearColor::White);

	void SetBottomText(const FText& str, FLinearColor c = FLinearColor::White);

	void SetHideBottomText();

	void SetHideTopText();

	virtual const FColorDataRow& GetColorData() const;

	void SetColorTier(const FColorDataRow& colorRow);

	void SetIcon(UTexture2D* icon);

	virtual  void Clear();

protected:
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeDestruct() override;

	virtual void NativePreConstruct() override;
};
