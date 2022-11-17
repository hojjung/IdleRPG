
#include "Widgets/GameLevel/MainMenu/Map/WidgetMapSelect.h"
#include "ConquerMap/ContentData.h"
#include "Manager/StageMode/RaidStageMode.h"
#include "Widgets/GameLevel/MainMenu/Map/WidgetMapInfo.h"
#include "Widgets/GameLevel/MainMenu/Map/WidgetMapSelectEle.h"

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

	for(const FContentMobData& MobData : m_ContentData->m_AryLevel)
	{
		UWidgetMapSelectEle* Ele = CreateWidget<UWidgetMapSelectEle>(this, m_ClassEle);

		URaidStageMode* DoStage = URaidStageMode::StaticClass()->GetDefaultObject<URaidStageMode>();
		
		FText StageName = DoStage->GetStageName(MobData.m_nLevel);

		Ele->SetZone(this, MobData.m_nLevel, StageName);
		//
		// m_AryEles.Add(Ele);

		m_Scroll->AddChild(Ele);

		Ele->SetPadding(FMargin(25,10,25,10));
	}
	SetInfoPanel(m_ContentData->m_AryLevel[0].m_nLevel);
}

void UWidgetMapSelect::SetInfoPanel(int lv)
{
	m_MapInfo->SetInfo(m_ContentData, lv);
}
