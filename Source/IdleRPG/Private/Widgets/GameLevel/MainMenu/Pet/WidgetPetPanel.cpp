#include "Widgets/GameLevel/MainMenu/Pet/WidgetPetPanel.h"
#include "MyAssetManager.h"
#include "Manager/MyGameInstance.h"
#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"
#include "Widgets/Lib/WidgetLib.h"

void UWidgetPetPanel::NativeOnInitialized()
{
	m_PetManager = TWeakPtr<PetManager>(UMyGameInstance::Get->m_PetManager);

	m_PetManager.Pin()->m_PetInven->SpawnPreviewActor(GetWorld(), 300, -80);

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
	//
	m_BtnAll->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnClickAll);
	m_BtnTier1->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnClickTier1);
	m_BtnTier2->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnClickTier2);
	m_BtnTier3->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnClickTier3);
	m_BtnTier4->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnClickTier4);
	m_BtnTier5->OnClicked.AddDynamic(this, &UWidgetPetPanel::OnClickTier5);

	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnAll);
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
}

void UWidgetPetPanel::OnSelectLoaded(FName key, const FPetDataRow* row)
{
	FPrimaryAssetId Id (TEXT("Unit"), key);
	
	UUnitAsset* Asset = Cast<UUnitAsset>(UMyAssetManager::Get()->GetPrimaryAssetObject(Id));

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
	m_MapEle.Reserve(50);
	UPetData::GetPetData->ForeachRow<FPetDataRow>("", [&](const FName& key, const FPetDataRow& row)
	{
		OnAvatarLoaded(key, row);
	});
	SortAvatar();
}

void UWidgetPetPanel::OnAvatarLoaded(const FName& key, const FPetDataRow& row)
{
	UWidgetPetEle* Ele = CreateWidget<UWidgetPetEle>(this, m_ClassEle);

	Ele->SetDataOnClick(key, row, UWidgetItemEle::FOnClick::CreateUObject(this, &UWidgetPetPanel::OnSelect));

	m_AryEles.Add(Ele);

	FName ColorID = row.m_Color.RowName;

	TArray<UWidgetPetEle*>* AryFound = m_MapEle.Find(ColorID);

	if(AryFound)
	{
		(*AryFound).Add(Ele);
	}
	else
	{
		TArray<UWidgetPetEle*> Ary;
		Ary.Add(Ele);
		m_MapEle.Add(ColorID, Ary);	
	}
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
	
	m_Equip1->SetColorTier(ColorDataFound);
	m_Equip2->SetColorTier(ColorDataFound);
	m_Equip3->SetColorTier(ColorDataFound);
	m_Equip4->SetColorTier(ColorDataFound);
	
	m_Equip1->Clear();
	m_Equip2->Clear();
	m_Equip3->Clear();
	m_Equip4->Clear();
}

void UWidgetPetPanel::SetColor(const FColorDataRow& ColorData)
{
	m_ImgGlow->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextTierName->SetColorAndOpacity(ColorData.m_Color.GetSpecifiedColor());

	m_TextTierName->SetText(ColorData.m_Name);
}

void UWidgetPetPanel::Filter(FName colorID)
{
	for(UWidgetPetEle* Ele : m_AryEles)
	{
		Ele->SetVisibility(ESlateVisibility::Collapsed);
	}

	TArray<UWidgetPetEle*>* Ary = m_MapEle.Find(colorID);
	
	if(!Ary)
	{
		return;
	}
	
	TArray<UWidgetPetEle*>& AryEles = *Ary;
	
	for(UWidgetPetEle* Ele : AryEles)
	{
		Ele->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}	
}

void UWidgetPetPanel::NoFilter()
{
	for(UWidgetPetEle* Ele : m_AryEles)
	{
		Ele->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetPetPanel::Equip(int index, UWidgetPetEle* btn)
{
	m_PetManager.Pin()->m_PetInven->EquipPet(index);

	FName KeyEquip;
	
	const FPetDataRow* Row;
	
	m_PetManager.Pin()->m_PetInven->GetKeyEquip(index, KeyEquip, &Row);
	
	btn->SetData(KeyEquip, *Row);
}

void UWidgetPetPanel::OnClickEquip1(const FName& key, const FEntityDataRow& row)
{
	Equip(0, m_Equip1);
}
void UWidgetPetPanel::OnClickEquip2(const FName& key, const FEntityDataRow& row)
{
	Equip(1, m_Equip2);
}
void UWidgetPetPanel::OnClickEquip3(const FName& key, const FEntityDataRow& row)
{
	Equip(2, m_Equip3);
}
void UWidgetPetPanel::OnClickEquip4(const FName& key, const FEntityDataRow& row)
{
	Equip(3, m_Equip4);
}

void UWidgetPetPanel::OnClickAll()
{
	if(m_CurrentBtn.Get() == m_BtnAll)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnAll);

	NoFilter();
}

void UWidgetPetPanel::OnClickTier1()
{
	if(m_CurrentBtn.Get() == m_BtnTier1)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier1);

	Filter(TEXT("Default"));
}

void UWidgetPetPanel::OnClickTier2()
{
	if(m_CurrentBtn.Get() == m_BtnTier2)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier2);

	Filter(TEXT("Uncommon"));
}

void UWidgetPetPanel::OnClickTier3()
{
	if(m_CurrentBtn.Get() == m_BtnTier3)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier3);

	Filter(TEXT("Rare"));
}

void UWidgetPetPanel::OnClickTier4()
{
	if(m_CurrentBtn.Get() == m_BtnTier4)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier4);

	Filter(TEXT("Hero"));
}

void UWidgetPetPanel::OnClickTier5()
{
	if(m_CurrentBtn.Get() == m_BtnTier5)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier5);

	Filter(TEXT("Legend"));
}

void UWidgetPetPanel::OnSelect(const FName& key, const FEntityDataRow& row)
{
	const FPetDataRow& AvRow = *(const FPetDataRow*)&row;
	
	FPrimaryAssetId Id (TEXT("Unit"), key);
	//
	m_PetManager.Pin()->m_PetInven->ChangeAvatar(Id,FStreamableDelegate::CreateUObject(this, &UWidgetPetPanel::OnSelectLoaded, key, &AvRow));
}
