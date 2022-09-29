// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/WidgetMainCanvas.h"

void UWidgetMainCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnAvatar->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickAvatarMenu);

	m_BtnUpgrade->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnOpenUpgradePanel);
}

void UWidgetMainCanvas::OnClickAvatarMenu()
{
	m_AvatarPanel->OnOpen();
}

void UWidgetMainCanvas::OnOpenUpgradePanel()
{
	m_UpgradePanel->OnOpen();
}
