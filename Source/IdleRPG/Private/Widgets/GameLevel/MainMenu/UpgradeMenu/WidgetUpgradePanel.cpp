#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetUpgradePanel.h"

void UWidgetUpgradePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UUpgradeData::GetUpgradeTable->ForeachRow<FUpgradeDataTableRow>("",[=](const FName& key, const FUpgradeDataTableRow& row)
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
