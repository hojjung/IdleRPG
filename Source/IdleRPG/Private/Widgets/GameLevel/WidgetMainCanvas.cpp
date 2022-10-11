#include "Widgets/GameLevel/WidgetMainCanvas.h"

void UWidgetMainCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnUpgrade->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnOpenUpgradePanel);

	m_BtnSkill->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickSkillPanel);

	m_BtnInventory->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickInvenPanel);

	m_BtnPet->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickPetMenu);

	m_BtnAvatar->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickAvatarMenu);

	m_BtnMap->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickMapPanel);

	m_BtnAcce->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickAccePanel);

	m_BtnRelic->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickRelicPanel);

	m_BtnShop->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickShopPanel);
}

void UWidgetMainCanvas::OnOpenUpgradePanel()
{
	m_UpgradePanel->OnOpen();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();
}
void UWidgetMainCanvas::OnClickSkillPanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnOpen();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();
}
void UWidgetMainCanvas::OnClickInvenPanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnOpen();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();
}

void UWidgetMainCanvas::OnClickPetMenu()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnOpen();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();
}

void UWidgetMainCanvas::OnClickAvatarMenu()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnOpen();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();
}

void UWidgetMainCanvas::OnClickMapPanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnOpen();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();
}

void UWidgetMainCanvas::OnClickAccePanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnOpen();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();
}

void UWidgetMainCanvas::OnClickRelicPanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnOpen();
	m_ShopPanel->OnClose();
}



void UWidgetMainCanvas::OnClickShopPanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnOpen();
}








