// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/WidgetMainCanvas.h"

void UWidgetMainCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnAvatar->m_OnClick.BindUObject(this, &UWidgetMainCanvas::OnClickAvatarMenu);
}

void UWidgetMainCanvas::OnClickAvatarMenu()
{
	m_AvatarPanel->OnOpen();
}
