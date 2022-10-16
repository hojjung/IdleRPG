// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WidgetPopupText.generated.h"

class UPopupTextWComp;
/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetPopupText : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UWidgetPopupText(const FObjectInitializer& objInit);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;

public:
	TWeakObjectPtr<UPopupTextWComp> m_ParentComponent;
	
	void SetTextWant(const FText& textWant, int index);
};
