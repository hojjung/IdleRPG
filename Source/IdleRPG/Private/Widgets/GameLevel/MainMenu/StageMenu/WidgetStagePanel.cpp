#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStagePanel.h"
#include "Monsters/StageTable.h"

void UWidgetStagePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnLeft->OnClicked.AddDynamic(this, &UWidgetStagePanel::OnClickLeft);
	m_BtnRight->OnClicked.AddDynamic(this, &UWidgetStagePanel::OnClickRight);

	m_nStageLevel = 0;

	m_AryEles.Reserve(20);

	UMyGameInstance::Get->m_OnMapChange.AddUObject(this, &UWidgetStagePanel::OnMapChanged);

	UStageTable::GetDefaultStage->GetAllRows("", m_AryRows);

	m_nMaxStage = m_AryRows.Num() - 1;

	UpdateStage();
}

void UWidgetStagePanel::OnMapChanged(EGameMode m, int level)
{
	m_nStageLevel = level;
}

void UWidgetStagePanel::UpdateStage()
{
	Clear();
	
	const FStageRow* StageRowFound = m_AryRows[m_nStageLevel];

	m_ImgIcon->SetBrushFromTexture(StageRowFound->m_IconStage);

	m_TextName->SetText(StageRowFound->m_StageName);

	int Level = 0;

	for(const FZone& Z : StageRowFound->m_AryUnits)
	{
		UWidgetStageEle* Ele = CreateWidget<UWidgetStageEle>(this, m_ClassEle);

		Ele->SetZone(StageRowFound->m_StageName, Level++);

		m_AryEles.Add(Ele);

		m_Scroll->AddChild(Ele);

		Ele->SetPadding(FMargin(25));
	}
}

void UWidgetStagePanel::Clear()
{
	for (TWeakObjectPtr<UWidgetStageEle> Ele : m_AryEles)
	{
		Ele->SetVisibility(ESlateVisibility::Collapsed);
		Ele->RemoveFromParent();
	}
}

void UWidgetStagePanel::OnClickLeft()
{
	m_nStageLevel--;
	
	m_nStageLevel = FMath::Max(m_nStageLevel, 0);

	UpdateStage();
}

void UWidgetStagePanel::OnClickRight()
{
	m_nStageLevel++;
	
	m_nStageLevel = FMath::Min(m_nStageLevel, m_nMaxStage);

	UpdateStage();
}
