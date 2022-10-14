// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetItemInfo.h"
#include "Blueprint/UserWidget.h"
#include "WidgetConsumInven.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetConsumInven : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetItemInfo* m_ItemInfo;
	
public:
	void OnOpen();

	void OnClose();
};
