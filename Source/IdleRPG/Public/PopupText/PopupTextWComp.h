// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupTextWidget.h"
#include "Components/WidgetComponent.h"
#include "PopupTextWComp.generated.h"

class UWidgetPopupText;
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
	TSubclassOf<UWidgetPopupText> m_ClassTextWidget;

	TWeakObjectPtr<UWidgetPopupText> m_TextWidget;
	
protected:
	virtual void BeginPlay() override;

public:
	void EndAnimation();
	
	void SetTextWant(const FVector& loc, const FText& text, int index);
};
