// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WidgetStageEle.generated.h"

class UWidgetStagePanel;
/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetStageEle : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnEnter;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextPrize;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextName;

	int m_nLevel;

	TWeakObjectPtr<UWidgetStagePanel> m_Parent;
	
public:
	void SetZone(UWidgetStagePanel* parent,int level);

	UFUNCTION()
	void OnEnter();
};
