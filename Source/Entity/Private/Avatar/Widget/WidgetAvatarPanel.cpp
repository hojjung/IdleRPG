#include "Avatar/Widget/WidgetAvatarPanel.h"
#include "Entity.h"
#include "MyAssetManager.h"
#include "DataTableRow/AvatarData.h"

void UWidgetAvatarPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClose);
	
	CreateAllElements();
	
}

void UWidgetAvatarPanel::CreateAllElements()
{
	TArray<FAvatarRow*> AryAvatarRow;
	
	UAvatarData::GetAvatarTable->GetAllRows("", AryAvatarRow);

	for(const FAvatarRow* AvatarRow : AryAvatarRow)
	{
		UWidgetAvatarEle* Ele = CreateWidget<UWidgetAvatarEle>(this, m_ClassEle);
		
		Ele->Init(AvatarRow);

		Ele->m_OnClick.BindUObject(this, &UWidgetAvatarPanel::OnSelect);

		m_Wrap->AddChild(Ele);
	}
}

void UWidgetAvatarPanel::OnSelect(const FAvatarRow* row)
{
	FEntityModule::Get().GetAvatarManager()->m_AvatarInven->ChangeAvatar(row->m_EntityAsset, FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnSelectLoaded, row));
}

void UWidgetAvatarPanel::OnSelectLoaded(const FAvatarRow* row)
{
	UUnitAsset* Asset = Cast<UUnitAsset>(UMyAssetManager::Get()->GetPrimaryAssetObject(row->m_EntityAsset));
	//셀렉트는 이미 끝났고
	//프리뷰 메쉬 생성
	//아이콘 생성
	PRINTF("ID:%s", *Asset->m_BodyMesh.Get()->GetName());

	const FColorDataRow* ColorData = row->m_ColorData.GetRow<FColorDataRow>("");
	
	m_ImgGlow->SetColorAndOpacity(ColorData->m_Color.GetSpecifiedColor());

	m_TextName->SetColorAndOpacity(ColorData->m_Color.GetSpecifiedColor());

	m_TextTierName->SetColorAndOpacity(ColorData->m_Color.GetSpecifiedColor());

	m_TextName->SetText(Asset->m_ShowingName);
	
	m_TextTierName->SetText(ColorData->m_Name);
}

void UWidgetAvatarPanel::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetAvatarPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
