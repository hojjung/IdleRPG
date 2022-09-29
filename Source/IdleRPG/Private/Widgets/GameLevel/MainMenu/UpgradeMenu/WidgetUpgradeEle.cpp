#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetUpgradeEle.h"

void UWidgetUpgradeEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidgetUpgradeEle::SetUpgradeData(const FUpgradeDataTableRow& row)
{
	m_UpgradeRow = &row;

	m_TextMaxLevel->SetText(FText::AsNumber(m_UpgradeRow->m_nMaxLevel));
	
	m_ImgIcon->SetBrushFromTexture(m_UpgradeRow->m_Icon);

	//SetLevel(0);
}

void UWidgetUpgradeEle::SetUpgradeData(const FLevelUpDataTableRow& row)
{
	m_LevelUpRow = &row;

	m_TextMaxLevel->SetText(FText::AsNumber(m_LevelUpRow->m_nMaxLevel));
	
	m_ImgIcon->SetBrushFromTexture(m_LevelUpRow->m_Icon);

	//SetLevel(0);
}

void UWidgetUpgradeEle::SetLevel(int lv)
{
	//FString FormatStr = FString::Printf(TEXT("%d"), lv);
	
	m_TextDesc->SetText(m_UpgradeRow->m_StrUpgradeDesc);
}
