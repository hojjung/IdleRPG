#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"
#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStagePanel.h"
#include "Manager/MyGameInstance.h"

void UWidgetStageEle::SetZone(UUserWidget* parent,int level, FText stageName, EGameMode mode)
{
	m_Parent = parent;
	
	m_nLevel = level;

	m_GameMode = mode;
	
	m_TextName->SetText(stageName);

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetStageEle::OnEnter);
}

void UWidgetStageEle::OnEnter()
{
	UMyGameInstance::Get->StartGameMode(m_GameMode, m_nLevel);

	m_Parent->SetVisibility(ESlateVisibility::Collapsed);
}
