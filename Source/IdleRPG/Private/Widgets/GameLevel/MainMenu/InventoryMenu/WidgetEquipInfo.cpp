// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipInfo.h"

void UWidgetEquipInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetEquipInfo::Close);
}

void UWidgetEquipInfo::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetEquipInfo::ShowInfo(const FEquipRow& equipRow)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//현재 레벨을 어떻게 가져오지?
}
