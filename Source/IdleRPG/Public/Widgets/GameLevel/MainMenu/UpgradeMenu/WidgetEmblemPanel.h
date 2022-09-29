// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetEmblemEle.h"
#include "Blueprint/UserWidget.h"
#include "WidgetEmblemPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetEmblemPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetEmblemEle> m_ClassEle;

public:
	void Open();
};
