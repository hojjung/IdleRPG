
#include "Widgets/GameLevel/MainMenu/Map/WidgetMapInfo.h"

void UWidgetMapInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetMapInfo::OnClick);
}

void UWidgetMapInfo::SetInfo(const FContentDataRow* content_data_row, int lv)
{
	m_ScrollPrize->ClearChildren();
	
	m_TextName->SetText(content_data_row->m_Name);

	m_TextDesc->SetText(content_data_row->m_Desc);

	m_nLv = lv;
	
	FText Str = FText::FromString(FString::Printf(TEXT("Stage Lv.%d"), m_nLv));
	
	m_TextLevel->SetText(Str);
	
	m_ImgIcon->SetBrushFromTexture(content_data_row->m_Icon);

	//열쇠 개수 업데이트
	//소탕권
	//dailymax
	//
	m_Mode = content_data_row->m_GameMode;

}

void UWidgetMapInfo::OnClick()
{
	UMyGameInstance::Get->StartGameMode(m_Mode, m_nLv);
	
	//m_Parent->SetVisibility(ESlateVisibility::Collapsed);
}
