// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/WidgetScreenEffect.h"
#include "WidgetScreenFadeCanvas.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetScreenFadeCanvas : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetScreenEffect* m_ScreenEffect;
	//

public:
	FORCEINLINE UWidgetScreenEffect* GetScreenEffect()
	{
		return  m_ScreenEffect;
	}
};
