#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStagePanel.h"

#include "Manager/StageMode/DefaultStageMode.h"

void UWidgetStagePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nMaxMap = UDefaultStageMode::AryStageRows.Num();
	
	m_BtnLeft->OnClicked.AddDynamic(this, &UWidgetStagePanel::OnClickLeft);
	
	m_BtnRight->OnClicked.AddDynamic(this, &UWidgetStagePanel::OnClickRight);

	UMyGameInstance::Get->m_MapChanged.AddUObject(this, &UWidgetStagePanel::OnMapChanged);

	OnMapChanged();
	
	UpdateStage();
}

void UWidgetStagePanel::OnMapChanged()
{
	int StageLevel = UMyGameInstance::Get->GetStageMode()->GetLevel();
	
	m_nMapIndex = UDefaultStageMode::GetDefaultStageMapIndex(StageLevel);
}

void UWidgetStagePanel::UpdateStage()
{
	Clear();

	const FStageRow& StageRowFound = *UDefaultStageMode::AryStageRows[m_nMapIndex];

	m_ImgIcon->SetBrushFromTexture(StageRowFound.m_IconStage);

	m_TextName->SetText(StageRowFound.m_StageName);

	int Level = m_nMapIndex * 20;

	int Iter = -1;
	while (++Iter < 20)
	{
		UWidgetStageEle* Ele = CreateWidget<UWidgetStageEle>(this, m_ClassEle);

		UDefaultStageMode* DoStage = UDefaultStageMode::StaticClass()->GetDefaultObject<UDefaultStageMode>();

		FText StageName = DoStage->GetStageName(Level);

		Ele->SetZone(this, Level, StageName);

		m_Scroll->AddChild(Ele);

		Ele->SetPadding(FMargin(25));

		Level++;
	}//
}

void UWidgetStagePanel::Clear()
{
	m_Scroll->ClearChildren();
}

void UWidgetStagePanel::OnClickLeft()
{
	m_nMapIndex--;
	
	m_nMapIndex = FMath::Max(m_nMapIndex, 0);

	UpdateStage();
}

void UWidgetStagePanel::OnClickRight()
{
	m_nMapIndex++;
	
	m_nMapIndex = FMath::Min(m_nMapIndex, m_nMaxMap);

	UpdateStage();
}
