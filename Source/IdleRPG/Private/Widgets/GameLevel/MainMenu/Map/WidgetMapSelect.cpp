// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetMapSelect.h"

#include "ConquerMap/ContentData.h"
#include "Manager/StageMode/RaidStageMode.h"

void UWidgetMapSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetMapSelect::OnClose);
	
	OnClose();
}

void UWidgetMapSelect::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	m_ContentData = nullptr;
}

void UWidgetMapSelect::Open(const FName& id)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_ContentData = UContentData::GetContentData->FindRow<FContentDataRow>(id, "");

	m_Scroll->ClearChildren();

	for(int Level : m_ContentData->m_AryLevel)
	{
		UWidgetStageEle* Ele = CreateWidget<UWidgetStageEle>(this, m_ClassEle);

		URaidStageMode* DoStage = URaidStageMode::StaticClass()->GetDefaultObject<URaidStageMode>();
		
		FText StageName = DoStage->GetStageName(Level);
		
		Ele->SetZone(this, Level, StageName, m_ContentData->m_GameMode);
		//
		// m_AryEles.Add(Ele);

		m_Scroll->AddChild(Ele);

		Ele->SetPadding(FMargin(25,10,25,10));
	}
	SetInfoPanel(m_ContentData->m_AryLevel[0]);
}

void UWidgetMapSelect::SetInfoPanel(int lv)
{
	m_MapInfo->SetInfo(m_ContentData, lv);
}
