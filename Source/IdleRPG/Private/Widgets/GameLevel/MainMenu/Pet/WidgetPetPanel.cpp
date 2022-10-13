#include "Widgets/GameLevel/MainMenu/Pet/WidgetPetPanel.h"

#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"

void UWidgetPetPanel::NativeOnInitialized()
{
	m_PetManager = TWeakPtr<PetManager>(UMyGameInstance::Get->m_PetManager);

	m_PetManager.Pin()->m_PetInven->SpawnPreviewActor(GetWorld());

	CreateAllElements();

	m_Preview->Init(m_PetManager.Pin()->m_PetInven->GetPreviewActor());

	Super::NativeOnInitialized();

	ClearPanel();
	
	m_Equip1->GetOnClick().BindUObject(this, &UWidgetPetPanel::OnClickEquip1);
	
	m_Equip2->GetOnClick().BindUObject(this, &UWidgetPetPanel::OnClickEquip2);

	m_Equip3->GetOnClick().BindUObject(this, &UWidgetPetPanel::OnClickEquip3);

	m_Equip4->GetOnClick().BindUObject(this, &UWidgetPetPanel::OnClickEquip4);
	
	FName Key;
	
	const FPetDataRow* Row = nullptr;

	m_PetManager.Pin()->m_PetInven->GetKeyEquip(0, Key, &Row);
	
	m_Equip1->SetData(Key, *Row);

	m_PetManager.Pin()->m_PetInven->GetKeyEquip(1, Key, &Row);
	
	m_Equip2->SetData(Key, *Row);

	m_PetManager.Pin()->m_PetInven->GetKeyEquip(2, Key, &Row);
	
	m_Equip3->SetData(Key, *Row);

	m_PetManager.Pin()->m_PetInven->GetKeyEquip(3, Key, &Row);
	
	m_Equip4->SetData(Key, *Row);
}

void UWidgetPetPanel::NativeDestruct()
{
	Super::NativeDestruct();

	m_PetManager.Reset();
}

void UWidgetPetPanel::OnOpen()
{
	Super::OnOpen();

	m_PetManager.Pin()->m_PetInven->ShowPreview();

	FName Key;
	
	const FPetDataRow* Row = nullptr;

	m_PetManager.Pin()->m_PetInven->GetKeyVisualEquip(Key, &Row);

	if(Key != NAME_None)
	{
		OnSelectLoaded(Key, Row);
	}
}

void UWidgetPetPanel::OnSelectLoaded(FName key, const FPetDataRow* row)
{
	UUnitAsset* Asset = Cast<UUnitAsset>(UMyAssetManager::Get()->GetPrimaryAssetObject(row->m_EntityAsset));

	SetColor(row->GetColor());

	m_TextName->SetText(row->m_Name);

	m_PetManager.Pin()->m_PetInven->SetPreview(key, Asset);
}

void UWidgetPetPanel::OnClose()
{
	Super::OnClose();

	m_PetManager.Pin()->m_PetInven->HidePreview();
}


void UWidgetPetPanel::CreateAllElements()
{
	UPetData::GetPetData->ForeachRow<FPetDataRow>("", [&](const FName& key, const FPetDataRow& row)
	{
		OnAvatarLoaded(key, row);
	});
	SortAvatar();
}

void UWidgetPetPanel::OnAvatarLoaded(const FName& key, const FPetDataRow& row)
{
	UWidgetPetEle* Ele = CreateWidget<UWidgetPetEle>(this, m_ClassEle);

	Ele->SetData(key, row, UWidgetItemEle::FOnClick::CreateUObject(this, &UWidgetPetPanel::OnSelect));

	m_AryEles.Add(Ele);
}

void UWidgetPetPanel::SortAvatar()
{
	m_AryEles.Sort([](const UWidgetPetEle& LHS, const UWidgetPetEle& RHS)
	{
		int LhsLevel = LHS.GetSortOrder();

		int RhsLevel = RHS.GetSortOrder();

		return LhsLevel < RhsLevel;
	});

	for (UWidgetPetEle* Ele : m_AryEles)
	{
		m_Wrap->AddChildToWrapBox(Ele);
	}
}

void UWidgetPetPanel::ClearPanel()
{
	m_TextName->SetText(FText());

	const FColorDataRow& ColorDataFound = *UColorData::GetColorTable->FindRow<FColorDataRow>(TEXT("Default"), "");

	SetColor(ColorDataFound);
}

void UWidgetPetPanel::SetColor(const FColorDataRow& ColorData)
{
	m_ImgGlow->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextTierName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextTierName->SetText(ColorData.m_Name);
}

void UWidgetPetPanel::OnClickEquip1(const FName& key, const FEntityDataRow& row)
{
	// m_PetManager.Pin()->m_PetInven->EquipAvatar();
	//
	// FName KeyEquip = m_PetManager.Pin()->m_PetInven->GetKeyEquip();
	//
	// const FAvatarRow& RowEquip = m_PetManager.Pin()->m_PetInven->GetRowEquip();
	//
	// m_Equip1->SetData(KeyEquip, RowEquip);
}
void UWidgetPetPanel::OnClickEquip2(const FName& key, const FEntityDataRow& row)
{
	// m_PetManager.Pin()->m_PetInven->EquipAvatar();
	//
	// FName KeyEquip = m_PetManager.Pin()->m_PetInven->GetKeyEquip();
	//
	// const FAvatarRow& RowEquip = m_PetManager.Pin()->m_PetInven->GetRowEquip();
	//
	// m_Equip2->SetData(KeyEquip, RowEquip);
}
void UWidgetPetPanel::OnClickEquip3(const FName& key, const FEntityDataRow& row)
{
	// m_PetManager.Pin()->m_PetInven->EquipAvatar();
	//
	// FName KeyEquip = m_PetManager.Pin()->m_PetInven->GetKeyEquip();
	//
	// const FAvatarRow& RowEquip = m_PetManager.Pin()->m_PetInven->GetRowEquip();
	//
	// m_Equip3->SetData(KeyEquip, RowEquip);
}
void UWidgetPetPanel::OnClickEquip4(const FName& key, const FEntityDataRow& row)
{
	// m_PetManager.Pin()->m_PetInven->EquipAvatar();
	//
	// FName KeyEquip = m_PetManager.Pin()->m_PetInven->GetKeyEquip();
	//
	// const FAvatarRow& RowEquip = m_PetManager.Pin()->m_PetInven->GetRowEquip();
	//
	// m_Equip4->SetData(KeyEquip, RowEquip);
}
void UWidgetPetPanel::OnSelect(const FName& key, const FEntityDataRow& row)
{
	const FPetDataRow& AvRow = *(const FPetDataRow*)&row;
	//
	m_PetManager.Pin()->m_PetInven->ChangeAvatar(AvRow.m_EntityAsset,FStreamableDelegate::CreateUObject(this, &UWidgetPetPanel::OnSelectLoaded, key, &AvRow));
}