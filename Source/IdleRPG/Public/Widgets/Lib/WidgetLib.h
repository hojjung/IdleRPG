// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetLib.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void SetCurrentButton(TWeakObjectPtr<UButton>& current, UButton* btn)
	{
		if(current.Get())
		{
			FButtonStyle Sty = current->WidgetStyle;
			Sty.Normal.TintColor = FSlateColor(FLinearColor(0,0,0,1));
			Sty.Hovered.TintColor = Sty.Normal.TintColor;
			Sty.Pressed.TintColor = Sty.Normal.TintColor;
			Sty.Disabled.TintColor = Sty.Normal.TintColor;

			current->SetStyle(Sty);
		}

		current = btn;

		FButtonStyle Sty = current->WidgetStyle;
	
		Sty.Normal.TintColor = FSlateColor(FLinearColor(0.333333,0.251694,0.138765,1));
		Sty.Hovered.TintColor = Sty.Normal.TintColor;
		Sty.Pressed.TintColor = Sty.Normal.TintColor;
		Sty.Disabled.TintColor = Sty.Normal.TintColor;

		current->SetStyle(Sty);
	}
};

