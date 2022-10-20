#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetUpgradePanel.h"

void UWidgetUpgradePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UBigIntCalcTableBase::GetPlayerUpgradeTable->ForeachRow<FBigIntCalcTableRow>("",[=](const FName& key, const FBigIntCalcTableRow& row)
	{
		UWidgetUpgradeEle* Ele = CreateWidget<UWidgetUpgradeEle>(this, m_ClassEle);

		Ele->SetUpgradeData(row);

		m_ScrollBox->AddChild(Ele);
	});
}

void UWidgetUpgradePanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
