// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupTextWidget.h"
#include "Components/WidgetComponent.h"
#include "PopupTextWComp.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UPopupTextWComp : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UPopupTextWComp();

protected:
	virtual void BeginPlay() override;

	TSharedPtr<SPopupText> m_PopupText;
	
	
public:
	void SetTextWant(const FVector& loc, const FText& text, int index);
};
