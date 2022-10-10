// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Equipment/EquipData.h"
#include "WidgetEquipInfo.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetEquipInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void Close();

	void ShowInfo(const FEquipRow& equipRow);
};
