// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DataTableRow/AvatarData.h"
#include "WidgetAvatarEle.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UWidgetAvatarEle : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgTier;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgPortrait;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgWeaponType;

protected:
	const FAvatarRow* m_Row;
	
protected:
	virtual void NativeOnInitialized() override;

	const FColorDataRow& GetColorData() const;
public:
	void Init(const FAvatarRow* row);
};


