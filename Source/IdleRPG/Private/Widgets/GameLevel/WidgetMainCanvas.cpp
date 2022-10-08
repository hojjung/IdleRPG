#include "Widgets/GameLevel/WidgetMainCanvas.h"

void UWidgetMainCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnAvatar->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickAvatarMenu);

	m_BtnUpgrade->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnOpenUpgradePanel);
	
	m_BtnInventory->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnInvenPanel);
}

void UWidgetMainCanvas::OnClickAvatarMenu()
{
	m_AvatarPanel->OnOpen();
	m_UpgradePanel->OnClose();
	m_InvenPanel->OnClose();
}

void UWidgetMainCanvas::OnOpenUpgradePanel()
{
	m_AvatarPanel->OnClose();
	m_UpgradePanel->OnOpen();
	m_InvenPanel->OnClose();
}

void UWidgetMainCanvas::OnInvenPanel()
{
	m_UpgradePanel->OnClose();
	m_AvatarPanel->OnClose();
	m_InvenPanel->OnShow();
}
