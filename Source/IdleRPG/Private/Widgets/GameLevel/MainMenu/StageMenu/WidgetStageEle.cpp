#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"
#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStagePanel.h"
#include "Manager/MyGameInstance.h"

void UWidgetStageEle::SetZone(UWidgetStagePanel* parent, int level)
{
	m_Parent = parent;
	
	m_nLevel = level;

	FText StageName = UMyGameInstance::Get->GetStageMode()->GetStageName(m_nLevel);

	m_TextName->SetText(StageName);

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetStageEle::OnEnter);
}

void UWidgetStageEle::OnEnter()
{
	FVoidvoidMulti Empty;
	
	UMyGameInstance::Get->StartGameMode(EGameMode::Default, m_nLevel, Empty);

	m_Parent->SetVisibility(ESlateVisibility::Collapsed);
}
