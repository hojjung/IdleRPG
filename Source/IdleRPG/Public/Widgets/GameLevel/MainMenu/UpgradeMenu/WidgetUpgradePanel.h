// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetUpgradeEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "WidgetUpgradePanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetUpgradePanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FName> m_AryUpgradeKeys;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetUpgradeEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* m_ScrollBox;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void Open();
};


