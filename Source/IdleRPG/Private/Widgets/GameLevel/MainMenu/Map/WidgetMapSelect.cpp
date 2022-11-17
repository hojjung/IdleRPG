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
}

void UWidgetMapSelect::Open(const FName& id)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FContentDataRow* ContentData = UContentData::GetContentData->FindRow<FContentDataRow>(id, "");

	m_Scroll->ClearChildren();

	for(int Level : ContentData->m_AryLevel)
	{
		UWidgetStageEle* Ele = CreateWidget<UWidgetStageEle>(this, m_ClassEle);

		URaidStageMode* DoStage = URaidStageMode::StaticClass()->GetDefaultObject<URaidStageMode>();
		
		FText StageName = DoStage->GetStageName(Level);
		
		Ele->SetZone(this, Level, StageName, ContentData->m_GameMode);
		//
		// m_AryEles.Add(Ele);

		m_Scroll->AddChild(Ele);

		Ele->SetPadding(FMargin(25,10,25,10));

		Level++;
	}
}
