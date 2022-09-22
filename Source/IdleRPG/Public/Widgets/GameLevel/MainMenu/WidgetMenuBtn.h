// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMenuBtn.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMenuBtn : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnClick)

	FOnClick m_OnClick;

protected:
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
