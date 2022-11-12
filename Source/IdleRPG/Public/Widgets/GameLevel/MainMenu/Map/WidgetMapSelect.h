// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetMapSelect.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMapSelect : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;

protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClose();
	
	void Open(const FName& id);
};
