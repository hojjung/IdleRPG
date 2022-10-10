#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetInvenPanel.h"

#include "Widgets/Lib/WidgetLib.h"

void UWidgetInvenPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_EquipWeapon->Init(NSLOCTEXT("UWidgetInvenPanel", "Sword", "검"), UEquipData::GetWeaponData, 100);
	
	FText ArmorT = NSLOCTEXT("UWidgetInvenPanel", "Armor", "갑옷");
	
	m_EquipHelmet->Init(ArmorT, UEquipData::GetHelmetData, 30);
	m_EquipTorso->Init(ArmorT, UEquipData::GetTorsoData, 30);
	m_EquipGlove->Init(ArmorT, UEquipData::GetGloveData, 30);
	m_EquipLeg->Init(ArmorT, UEquipData::GetLegData, 30);
	m_EquipFeet->Init(ArmorT, UEquipData::GetFeetData, 30);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClose);

	m_BtnWeapon->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickWeapon);
	m_BtnArmor1->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor1);
	m_BtnArmor2->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor2);
	m_BtnArmor3->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor3);
	m_BtnArmor4->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor4);
	m_BtnArmor5->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor5);

	OnClickWeapon();
	OnClose();
}


void UWidgetInvenPanel::OnShow()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_EquipWeapon->OnShow();
}

void UWidgetInvenPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
}

void UWidgetInvenPanel::OnClickWeapon()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnWeapon);
	
	m_EquipWeapon->OnShow();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
}

void UWidgetInvenPanel::OnClickArmor1()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor1);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnShow();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
}

void UWidgetInvenPanel::OnClickArmor2()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor2);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnShow();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
}

void UWidgetInvenPanel::OnClickArmor3()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor3);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnShow();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
}

void UWidgetInvenPanel::OnClickArmor4()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor4);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnShow();
	m_EquipFeet->OnClose();
}

void UWidgetInvenPanel::OnClickArmor5()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor5);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnShow();
}
