// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetMapSelect.h"
#include "WidgetWorldMap.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetMenuMap.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMenuMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetWorldMap* m_Map;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMapSelect* m_Select;
protected:
	virtual void NativeOnInitialized() override;
	
public:
	void OnOpen();
	UFUNCTION()
	void OnClose();
};
