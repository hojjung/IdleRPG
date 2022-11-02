// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WidgetMapEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMapEle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_OneParam(FOnClick, const FName&);

	FOnClick m_OnClick;
	
protected:
	UPROPERTY(EditAnywhere)
	FName m_ZoneID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateIcon();

	virtual void NativePreConstruct() override;
	
public:
	UFUNCTION()
	void OnClick();
	
	FName GetZoneID();
	
	void SetZoneID(FName id, float dur);
	
	FVector2D GetPos();
};
