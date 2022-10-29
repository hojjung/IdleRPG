#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"
#include "Manager/MyGameInstance.h"
#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStagePanel.h"

void UWidgetStageEle::SetZone(UWidgetStagePanel* parent, int level)
{
	m_Parent = parent;
	
	m_nLevel = level;

	FText StageName = UMyGameInstance::Get->GetGameMode()->GetStageName(m_nLevel);

	m_TextName->SetText(StageName);

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetStageEle::OnEnter);
}

void UWidgetStageEle::OnEnter()
{
	FVoidvoid Empty;
	
	UMyGameInstance::Get->StartGameMode(EGameMode::Default, m_nLevel, Empty);

	m_Parent->SetVisibility(ESlateVisibility::Collapsed);
}
