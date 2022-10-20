#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetLevelStatPanel.h"

void UWidgetLevelStatPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UBigIntCalcTableBase::GetLevelUpTable->ForeachRow<FBigIntCalcTableRow>("",[=](const FName& key, const FBigIntCalcTableRow& row)
	{
		UWidgetUpgradeEle* Ele = CreateWidget<UWidgetUpgradeEle>(this, m_ClassEle);

		Ele->SetUpgradeData(row);

		m_ScrollBox->AddChild(Ele);
	});
}

void UWidgetLevelStatPanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
