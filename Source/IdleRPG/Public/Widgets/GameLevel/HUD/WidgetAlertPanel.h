// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetAlertText.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "WidgetAlertPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetAlertPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetAlertText> m_ClassInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_OverlayInfo;
	UPROPERTY()
	TArray<UWidgetAlertText*> m_AryInfo;
	UPROPERTY()
	int m_nIndex;
	
protected:
	virtual void NativeOnInitialized() override;

	UWidgetAlertText* GetTextElement();
	
public:
	void ShowText(const FText& str, const FLinearColor& color);
};
