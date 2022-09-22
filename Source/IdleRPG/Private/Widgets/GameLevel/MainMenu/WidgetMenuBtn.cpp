// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/WidgetMenuBtn.h"

FReply UWidgetMenuBtn::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	
	m_OnClick.ExecuteIfBound();

	return FReply::Handled();
}
