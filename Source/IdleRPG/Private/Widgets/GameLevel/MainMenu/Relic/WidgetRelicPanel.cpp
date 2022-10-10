#include "Widgets/GameLevel/MainMenu/Relic/WidgetRelicPanel.h"
#include "Relic/RelicData.h"

void UWidgetRelicPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_GachaBtn->Init(NSLOCTEXT("UWidgetRelicPanel", "GachaName", "유물"), 400);
	m_GachaBtn->HideBottom();

	URelicData::GetRelicData->ForeachRow<FRelicRow>("", [&](const FName& key, const FRelicRow& row)
	{
		UWidgetRelicEle* Ele = CreateWidget<UWidgetRelicEle>(this, m_ClassEle);

		Ele->SetRelicData(row, UWidgetRelicEle::FOnClick::CreateUObject(this, &UWidgetRelicPanel::OnUpgradeRelic));

		m_AryEles.Add(Ele);
	});

	m_AryEles.Sort([](const UWidgetRelicEle& LHS, const UWidgetRelicEle& RHS)
	{
		int LhsLevel = LHS.GetSortOrder();

		int RhsLevel = RHS.GetSortOrder();

		return LhsLevel < RhsLevel;
	});

	for (UWidgetRelicEle* Ele : m_AryEles)
	{
		m_Scroll->AddChild(Ele);
	}

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetRelicPanel::OnClose);

	OnClose();
}

void UWidgetRelicPanel::OnUpgradeRelic(const FRelicRow& row)
{
	
}

void UWidgetRelicPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetRelicPanel::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
