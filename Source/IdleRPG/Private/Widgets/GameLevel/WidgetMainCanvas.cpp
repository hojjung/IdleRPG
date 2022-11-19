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

void UWidgetMainCanvas::TryOpen(UWidgetMenuBase* menu)
{
	if(menu->IsOpened())
	{
		menu->OnClose();
		return;
	}
	menu->OnOpen();
}

void UWidgetMainCanvas::OnOpenUpgradePanel()
{
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();

	TryOpen(m_UpgradePanel);
}
void UWidgetMainCanvas::OnClickSkillPanel()
{
	m_UpgradePanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();

	TryOpen(m_SkillPanel);
}
void UWidgetMainCanvas::OnClickInvenPanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();

	TryOpen(m_InvenPanel);
}

void UWidgetMainCanvas::OnClickPetMenu()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();

	TryOpen(m_PetPanel);
}

void UWidgetMainCanvas::OnClickAvatarMenu()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_MapPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();

	TryOpen(m_AvatarPanel);
}

void UWidgetMainCanvas::OnClickMapPanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_AccePanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();

	TryOpen(m_MapPanel);
}

void UWidgetMainCanvas::OpenDeadAlert()
{
	m_DeadAlert->Init(this);
}

void UWidgetMainCanvas::OnClickAccePanel()
{
	m_UpgradePanel->OnClose();
	m_SkillPanel->OnClose();
	m_InvenPanel->OnClose();
	m_PetPanel->OnClose();
	m_AvatarPanel->OnClose();
	m_MapPanel->OnClose();
	m_RelicPanel->OnClose();
	m_ShopPanel->OnClose();

	TryOpen(m_AccePanel);
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
	m_ShopPanel->OnClose();

	TryOpen(m_RelicPanel);
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
	
	TryOpen(m_ShopPanel);
}