#include "Widgets/GameLevel/MainMenu/Avatar/WidgetAvatarPanel.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"

void UWidgetAvatarPanel::NativeOnInitialized()
{
	m_AvatarManager =TWeakPtr<AvatarManager>(UMyGameInstance::Get->m_AvatarManager);

	m_AvatarManager.Pin()->m_AvatarInven->SpawnPreviewActor(GetWorld());

	CreateAllElements();

	m_Preview->Init(m_AvatarManager.Pin()->m_AvatarInven->GetPreviewActor());
	
	Super::NativeOnInitialized();
}

void UWidgetAvatarPanel::NativeDestruct()
{
	Super::NativeDestruct();

	m_AvatarManager.Reset();
}

void UWidgetAvatarPanel::CreateAllElements()
{
	UAvatarData::GetAvatarTable->ForeachRow<FAvatarRow>("",[&](const FName& key, const FAvatarRow& row)
	{
		OnAvatarLoaded(key, row);
	});
	SortAvatar();
}

void UWidgetAvatarPanel::OnAvatarLoaded(const FName& key,const FAvatarRow& row)
{
	UWidgetAvatarEle* Ele = CreateWidget<UWidgetAvatarEle>(this, m_ClassEle);

	Ele->SetData(key, row, UWidgetItemEle::FOnClick::CreateUObject(this, &UWidgetAvatarPanel::OnSelect));

	m_AryEles.Add(Ele);
}

void UWidgetAvatarPanel::SortAvatar()
{
	m_AryEles.Sort([](const UWidgetAvatarEle& LHS, const UWidgetAvatarEle& RHS)
	{
		int LhsLevel = LHS.GetSortOrder();

		int RhsLevel = RHS.GetSortOrder();

		return LhsLevel < RhsLevel;
	});

	for(UWidgetAvatarEle* Ele : m_AryEles)
	{
		m_Wrap->AddChildToWrapBox(Ele);
	}
}

void UWidgetAvatarPanel::OnSelect(const FName& key, const FEntityDataRow& row)
{
	const FAvatarRow& AvRow = *(const FAvatarRow*)&row;
	
	m_AvatarManager.Pin()->m_AvatarInven->ChangeAvatar(AvRow.m_EntityAsset, FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnSelectLoaded, key, &AvRow));
}

void UWidgetAvatarPanel::OnSelectLoaded(FName key, const FAvatarRow* row)
{
	UUnitAsset* Asset = Cast<UUnitAsset>(UMyAssetManager::Get()->GetPrimaryAssetObject(row->m_EntityAsset));
	
	const FColorDataRow& ColorData = row->GetColor();
	
	m_ImgGlow->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextTierName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextName->SetText(row->m_Name);
	
	m_TextTierName->SetText(ColorData.m_Name);

	m_AvatarManager.Pin()->m_AvatarInven->SetPreview(key, Asset);
}

void UWidgetAvatarPanel::OnOpen()
{
	Super::OnOpen();

	m_AvatarManager.Pin()->m_AvatarInven->ShowPreview();
}

void UWidgetAvatarPanel::OnClose()
{
	Super::OnClose();

	m_AvatarManager.Pin()->m_AvatarInven->HidePreview();
}
