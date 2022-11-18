#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"
#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStagePanel.h"
#include "Manager/MyGameInstance.h"

void UWidgetStageEle::SetZone(UUserWidget* parent,int level, FText stageName)
{
	m_Parent = parent;
	
	m_nLevel = level;

	m_TextName->SetText(stageName);

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetStageEle::OnEnter);
}

void UWidgetStageEle::OnEnter()
{
	UMyGameInstance::Get->StartGameMode(m_nLevel, nullptr);

	m_Parent->SetVisibility(ESlateVisibility::Collapsed);
}
