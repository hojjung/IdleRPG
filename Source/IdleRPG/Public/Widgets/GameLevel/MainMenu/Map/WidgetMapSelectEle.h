// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetMapSelect.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Manager/MyGameInstance.h"
#include "WidgetMapSelectEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMapSelectEle : public UUserWidget
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

	TWeakObjectPtr<UWidgetMapSelect> m_Parent;
	
public:
	void SetZone(UWidgetMapSelect* parent,int level, FText stageName);

	UFUNCTION()
	void OnOpen();
};
