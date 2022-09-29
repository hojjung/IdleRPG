// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetClassEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "WidgetClassPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetClassPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetClassEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* m_ScrollBox;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void Open();
};
