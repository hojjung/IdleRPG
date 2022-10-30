// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WidgetCurrency.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetCurrency : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextCurrency;

public:
	void SetText(FText t);
};
