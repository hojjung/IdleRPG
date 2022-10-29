#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetUpgradePanel.h"

void UWidgetUpgradePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for(const FName& Key : m_AryUpgradeKeys)
	{
		const FBigIntCalcTableRow* Row = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(Key,"");

		UWidgetUpgradeEle* Ele = CreateWidget<UWidgetUpgradeEle>(this, m_ClassEle);

		Ele->SetUpgradeData(*Row);

		m_ScrollBox->AddChild(Ele);
	}
}

void UWidgetUpgradePanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
