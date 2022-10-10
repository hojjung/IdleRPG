// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetMenuMap.h"

void UWidgetMenuMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClose);

	OnClose();

	
}

void UWidgetMenuMap::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetMenuMap::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
