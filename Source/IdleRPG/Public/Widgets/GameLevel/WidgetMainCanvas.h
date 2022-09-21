// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Avatar/Widget/WidgetAvatarPanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetMainCanvas.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMainCanvas : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UWidgetAvatarPanel* m_AvatarPanel;
	UPROPERTY()
	UButton* m_BtnAvatar;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClickAvatarMenu();
};
