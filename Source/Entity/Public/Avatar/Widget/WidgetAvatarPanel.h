// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetAvatarEle.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAvatarPanel.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UWidgetAvatarPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetAvatarEle> m_ClassAvatarEle;

protected:
	virtual void NativeOnInitialized() override;
};
