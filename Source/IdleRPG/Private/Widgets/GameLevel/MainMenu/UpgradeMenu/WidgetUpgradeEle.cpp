#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetUpgradeEle.h"

void UWidgetUpgradeEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidgetUpgradeEle::SetUpgradeData(const FBigIntCalcTableRow& row)
{
	m_Row = &row;

	m_TextMaxLevel->SetText(FText::AsNumber(m_Row->m_nMaxLevel));
	
	m_ImgIcon->SetBrushFromTexture(m_Row->m_Icon);

	//SetLevel(0);
}

void UWidgetUpgradeEle::SetLevel(int lv)
{
	m_TextDesc->SetText(m_Row->GetDesc(lv));
}
