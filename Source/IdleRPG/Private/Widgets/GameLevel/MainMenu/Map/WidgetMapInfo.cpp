
#include "Widgets/GameLevel/MainMenu/Map/WidgetMapInfo.h"

#include "Manager/MyGameInstance.h"

void UWidgetMapInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetMapInfo::OnClick);
}

void UWidgetMapInfo::SetInfo(const FContentDataRow* content_data_row, int lv)
{
	m_ContentData = content_data_row;
	
	m_ScrollPrize->ClearChildren();
	
	m_TextName->SetText(m_ContentData->m_Name);

	m_TextDesc->SetText(m_ContentData->m_Desc);

	m_nLv = lv;
	
	FText Str = FText::FromString(FString::Printf(TEXT("Stage Lv.%d"), m_nLv));
	
	m_TextLevel->SetText(Str);
	
	m_ImgIcon->SetBrushFromTexture(m_ContentData->m_ConentIcon);

	//열쇠 개수 업데이트
	//소탕권
	//dailymax
	//
}

void UWidgetMapInfo::OnClick()
{
	UMyGameInstance::Get->StartGameMode(m_nLv, m_ContentData);
}
