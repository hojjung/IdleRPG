#include "Widgets/GameLevel/WidgetMainCanvas.h"

void UWidgetMainCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnAvatar->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickAvatarMenu);

	m_BtnUpgrade->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnOpenUpgradePanel);
	
	m_BtnInventory->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnInvenPanel);

	m_BtnRelic->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnRelicPanel);
}

void UWidgetMainCanvas::OnClickAvatarMenu()
{
	m_AvatarPanel->OnOpen();
	m_UpgradePanel->OnClose();
	m_InvenPanel->OnClose();

	m_RelicPanel->OnClose();
}

void UWidgetMainCanvas::OnOpenUpgradePanel()
{
	m_AvatarPanel->OnClose();
	m_UpgradePanel->OnOpen();
	m_InvenPanel->OnClose();

	m_RelicPanel->OnClose();
}

void UWidgetMainCanvas::OnInvenPanel()
{
	m_UpgradePanel->OnClose();
	m_AvatarPanel->OnClose();
	m_InvenPanel->OnShow();

	m_RelicPanel->OnClose();
}

void UWidgetMainCanvas::OnRelicPanel()
{
	m_RelicPanel->OnOpen();
	m_UpgradePanel->OnClose();
	m_AvatarPanel->OnClose();
	m_InvenPanel->OnClose();
}
