// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Player/Avatar/AvatarData.h"
#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"
#include "WidgetAvatarEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetAvatarEle : public UWidgetItemEle
{
	GENERATED_BODY()

public:
	virtual void SetData(const FName& id, const FEntityDataRow& dataEquip, FOnClick onClick) override;
};


