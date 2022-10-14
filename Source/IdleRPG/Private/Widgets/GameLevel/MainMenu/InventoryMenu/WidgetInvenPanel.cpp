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

	m_BtnConsumable->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickConsume);
	m_BtnWeapon->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickWeapon);
	m_BtnArmor1->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor1);
	m_BtnArmor2->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor2);
	m_BtnArmor3->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor3);
	m_BtnArmor4->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor4);
	m_BtnArmor5->OnClicked.AddDynamic(this, &UWidgetInvenPanel::OnClickArmor5);

	OnClickWeapon();
}


void UWidgetInvenPanel::OnOpen()
{
	Super::OnOpen();
	m_EquipWeapon->OnOpen();
}

void UWidgetInvenPanel::OnClose()
{
	Super::OnClose();
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
}

void UWidgetInvenPanel::OnClickConsume()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnConsumable);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();

	m_ConsumInven->OnOpen();
}

void UWidgetInvenPanel::OnClickWeapon()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnWeapon);
	
	m_EquipWeapon->OnOpen();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
	m_ConsumInven->OnClose();
}

void UWidgetInvenPanel::OnClickArmor1()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor1);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnOpen();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
	m_ConsumInven->OnClose();
}

void UWidgetInvenPanel::OnClickArmor2()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor2);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnOpen();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
	m_ConsumInven->OnClose();
}

void UWidgetInvenPanel::OnClickArmor3()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor3);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnOpen();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnClose();
	m_ConsumInven->OnClose();
}

void UWidgetInvenPanel::OnClickArmor4()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor4);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnOpen();
	m_EquipFeet->OnClose();
	m_ConsumInven->OnClose();
}

void UWidgetInvenPanel::OnClickArmor5()
{
	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnArmor5);
	
	m_EquipWeapon->OnClose();
	m_EquipHelmet->OnClose();
	m_EquipTorso->OnClose();
	m_EquipGlove->OnClose();
	m_EquipLeg->OnClose();
	m_EquipFeet->OnOpen();
	m_ConsumInven->OnClose();
}
