#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetClassEle.h"

void UWidgetClassEle::SetClassData(const FClassUpDataTableRow& row)
{
	m_ImgIcon->SetBrushFromTexture(row.m_Icon);
	
	//m_BtnUpgrade->OnClicked.
	
	m_TextDesc->SetText(row.m_StrUpgradeDesc);

	m_TextStat->SetText(row.m_StrUpgradeDesc);
}
