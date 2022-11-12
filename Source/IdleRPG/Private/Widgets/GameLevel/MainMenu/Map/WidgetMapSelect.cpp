// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetMapSelect.h"

#include "ConquerMap/ContentData.h"

void UWidgetMapSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetMapSelect::OnClose);
	
	OnClose();
}

void UWidgetMapSelect::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetMapSelect::Open(const FName& id)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FContentDataRow* ContentData = UContentData::GetContentData->FindRow<FContentDataRow>(id, "");

	
}
