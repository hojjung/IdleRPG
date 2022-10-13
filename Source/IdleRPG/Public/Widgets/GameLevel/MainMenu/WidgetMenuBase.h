// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetMenuBase.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMenuBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;
public:
	virtual void OnOpen();
	UFUNCTION()
	virtual void OnClose();

	bool IsOpened();

protected:
	virtual void NativeOnInitialized() override;
};
