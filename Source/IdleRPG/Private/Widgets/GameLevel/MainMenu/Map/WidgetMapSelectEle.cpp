#include "Widgets/GameLevel/MainMenu/Map/WidgetMapSelectEle.h"

void UWidgetMapSelectEle::SetZone(UWidgetMapSelect* parent,int level, FText stageName)
{
	m_Parent = parent;
	
	m_nLevel = level;

	m_TextName->SetText(stageName);

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetMapSelectEle::OnOpen);
}

void UWidgetMapSelectEle::OnOpen()
{
	m_Parent->SetInfoPanel(m_nLevel);
}
