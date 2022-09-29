#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetLevelStatPanel.h"
#include "Player/Upgrade/Data/LevelUpData.h"

void UWidgetLevelStatPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ULevelUpData::GetUpgradeTable->ForeachRow<FLevelUpDataTableRow>("",[=](const FName& key, const FLevelUpDataTableRow& row)
	{
		UWidgetUpgradeEle* Ele = CreateWidget<UWidgetUpgradeEle>(this, m_ClassEle);

		m_ScrollBox->AddChild(Ele);
	});
}

void UWidgetLevelStatPanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
