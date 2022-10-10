// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMapSelect.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMapSelect : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized() override;

public:
	void OnClose();
};
