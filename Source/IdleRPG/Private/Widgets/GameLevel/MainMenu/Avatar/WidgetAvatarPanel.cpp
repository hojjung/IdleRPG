#include "Widgets/GameLevel/MainMenu/Avatar/WidgetAvatarPanel.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"

void UWidgetAvatarPanel::NativeOnInitialized()
{
	m_AvatarManager =TWeakPtr<AvatarManager>(UMyGameInstance::Get->m_AvatarManager);

	m_AvatarManager.Pin()->m_AvatarInven->SpawnPreviewActor(GetWorld(), 450, -55);

	CreateAllElements();

	m_Preview->Init(m_AvatarManager.Pin()->m_AvatarInven->GetPreviewActor());
	
	Super::NativeOnInitialized();

	m_Equip->GetOnClick().BindUObject(this, &UWidgetAvatarPanel::OnClickEquip);

	m_Skin->GetOnClick().BindUObject(this, &UWidgetAvatarPanel::OnClickSkin);
	
	ClearPanel();

	FName KeyEquip = m_AvatarManager.Pin()->m_AvatarInven->GetKeyEquip();

	const FAvatarRow& RowEquip = m_AvatarManager.Pin()->m_AvatarInven->GetRowEquip();
		
	m_Equip->SetData(KeyEquip, RowEquip);

	FName KeySkin = m_AvatarManager.Pin()->m_AvatarInven->GetKeySkin();

	const FAvatarRow& RowSkin = m_AvatarManager.Pin()->m_AvatarInven->GetRowSkin();

	m_Skin->SetData(KeySkin, RowSkin);
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

void UWidgetAvatarPanel::ClearPanel()
{
	m_TextName->SetText(FText());

	const FColorDataRow& ColorDataFound = *UColorData::GetColorTable->FindRow<FColorDataRow>(TEXT("Default"), "");
	
	SetColor(ColorDataFound);
}

void UWidgetAvatarPanel::OnClickEquip(const FName& key, const FEntityDataRow& row)
{
	OnClickSkin(key, row);
		
	m_AvatarManager.Pin()->m_AvatarInven->EquipAvatar();
		
	FName KeyEquip = m_AvatarManager.Pin()->m_AvatarInven->GetKeyEquip();

	const FAvatarRow& RowEquip = m_AvatarManager.Pin()->m_AvatarInven->GetRowEquip();
		
	m_Equip->SetData(KeyEquip, RowEquip);
}

void UWidgetAvatarPanel::OnClickSkin(const FName& key, const FEntityDataRow& row)
{
	m_AvatarManager.Pin()->m_AvatarInven->EquipSkinAvatar();
		
	FName KeyEquip = m_AvatarManager.Pin()->m_AvatarInven->GetKeySkin();

	const FAvatarRow& RowEquip = m_AvatarManager.Pin()->m_AvatarInven->GetRowSkin();
		
	m_Skin->SetData(KeyEquip, RowEquip);
}

void UWidgetAvatarPanel::SetColor(const FColorDataRow& ColorData)
{
	m_ImgGlow->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextTierName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());
	
	m_TextTierName->SetText(ColorData.m_Name);
}

void UWidgetAvatarPanel::OnSelect(const FName& key, const FEntityDataRow& row)
{
	const FAvatarRow& AvRow = *(const FAvatarRow*)&row;
	
	m_AvatarManager.Pin()->m_AvatarInven->ChangeAvatar(AvRow.m_EntityAsset, FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnSelectLoaded, key, &AvRow));
}

void UWidgetAvatarPanel::OnSelectLoaded(FName key, const FAvatarRow* row)
{
	UUnitAsset* Asset = Cast<UUnitAsset>(UMyAssetManager::Get()->GetPrimaryAssetObject(row->m_EntityAsset));

	SetColor(row->GetColor());
	
	m_TextName->SetText(row->m_Name);

	m_AvatarManager.Pin()->m_AvatarInven->SetPreview(key, Asset);
}

void UWidgetAvatarPanel::OnOpen()
{
	Super::OnOpen();

	FName KeySkin = m_AvatarManager.Pin()->m_AvatarInven->GetKeySkin();

	const FAvatarRow& RowSkin = m_AvatarManager.Pin()->m_AvatarInven->GetRowSkin();
	
	OnSelectLoaded(KeySkin, &RowSkin);
}

void UWidgetAvatarPanel::OnClose()
{
	Super::OnClose();

	m_AvatarManager.Pin()->m_AvatarInven->HidePreview();
}
