// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WidgetAlertText.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetAlertText : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;

public:
	virtual void Show(const FText& str, FLinearColor color);
};
