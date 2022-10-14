// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetConsumInven.h"

void UWidgetConsumInven::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetConsumInven::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	//m_ItemInfo->Close();
}
