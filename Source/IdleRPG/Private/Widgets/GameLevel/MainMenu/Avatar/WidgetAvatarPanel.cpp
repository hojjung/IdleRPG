#include "Widgets/GameLevel/MainMenu/Avatar/WidgetAvatarPanel.h"
#include "MyAssetManager.h"
#include "Manager/MyGameInstance.h"
#include "Widgets/Lib/WidgetLib.h"

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
	//
	m_BtnAll->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClickAll);
	m_BtnTier1->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClickTier1);
	m_BtnTier2->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClickTier2);
	m_BtnTier3->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClickTier3);
	m_BtnTier4->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClickTier4);
	m_BtnTier5->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClickTier5);

	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnAll);
}

void UWidgetAvatarPanel::NativeDestruct()
{
	Super::NativeDestruct();

	m_AvatarManager.Reset();
}

void UWidgetAvatarPanel::CreateAllElements()
{
	m_MapEle.Reserve(50);
	UAvatarData::GetAvatarTable->ForeachRow<FAvatarRow>("",[&](const FName& key, const FAvatarRow& row)
	{
		OnAvatarLoaded(key, row);
	});
	SortAvatar();
}

void UWidgetAvatarPanel::OnAvatarLoaded(const FName& key,const FAvatarRow& row)
{
	UWidgetAvatarEle* Ele = CreateWidget<UWidgetAvatarEle>(this, m_ClassEle);

	Ele->SetDataOnClick(key, row, UWidgetItemEle::FOnClick::CreateUObject(this, &UWidgetAvatarPanel::OnSelect));

	m_AryEles.Add(Ele);

	FName ColorID = row.m_Color.RowName;

	TArray<UWidgetAvatarEle*>* AryFound = m_MapEle.Find(ColorID);

	if(AryFound)
	{
		(*AryFound).Add(Ele);
	}
	else
	{
		TArray<UWidgetAvatarEle*> Ary;
		Ary.Add(Ele);
		m_MapEle.Add(ColorID, Ary);	
	}
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

void UWidgetAvatarPanel::Filter(FName colorID)
{
	for(UWidgetAvatarEle* Ele : m_AryEles)
	{
		Ele->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	TArray<UWidgetAvatarEle*>* Ary = m_MapEle.Find(colorID);
	
	if(!Ary)
	{
		return;
	}
	
	TArray<UWidgetAvatarEle*>& AryEles = *Ary;
	
	for(UWidgetAvatarEle* Ele : AryEles)
	{
		Ele->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}	
}

void UWidgetAvatarPanel::NoFilter()
{
	for(UWidgetAvatarEle* Ele : m_AryEles)
	{
		Ele->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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

void UWidgetAvatarPanel::OnClickAll()
{
	if(m_CurrentBtn.Get() == m_BtnAll)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnAll);

	NoFilter();
}

void UWidgetAvatarPanel::OnClickTier1()
{
	if(m_CurrentBtn.Get() == m_BtnTier1)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier1);

	Filter(TEXT("Default"));
}

void UWidgetAvatarPanel::OnClickTier2()
{
	if(m_CurrentBtn.Get() == m_BtnTier2)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier2);

	Filter(TEXT("Uncommon"));
}

void UWidgetAvatarPanel::OnClickTier3()
{
	if(m_CurrentBtn.Get() == m_BtnTier3)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier3);

	Filter(TEXT("Rare"));
}

void UWidgetAvatarPanel::OnClickTier4()
{
	if(m_CurrentBtn.Get() == m_BtnTier4)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier4);

	Filter(TEXT("Hero"));
}

void UWidgetAvatarPanel::OnClickTier5()
{
	if(m_CurrentBtn.Get() == m_BtnTier5)
	{
		return;
	}
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnTier5);

	Filter(TEXT("Legend"));
}
