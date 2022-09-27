// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Player/Avatar/AvatarData.h"
#include "WidgetAvatarEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetAvatarEle : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClick, const FAvatarRow*)

	FOnClick m_OnClick;
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgTier;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgPortrait;

protected:
	const FAvatarRow* m_Row;
	
protected:
	virtual void NativeOnInitialized() override;
	
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	void Init(const FAvatarRow* row);
	
	const FColorDataRow& GetColorData() const;

	FORCEINLINE const FAvatarRow& GetAvatarDataRow()
	{
		return *m_Row;
	}
};


