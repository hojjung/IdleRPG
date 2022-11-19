// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/WidgetDefaultCanvas.h"

void UWidgetDefaultCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_StageBtn->GetBtn()->OnClicked.AddDynamic(this, &UWidgetDefaultCanvas::OnOpenStage);
}

void UWidgetDefaultCanvas::OnOpenStage()
{
	m_StagePanel->OnOpen();
}