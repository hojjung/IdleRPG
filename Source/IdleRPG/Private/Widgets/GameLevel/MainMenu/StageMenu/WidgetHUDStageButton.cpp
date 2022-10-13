// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetHUDStageButton.h"

#include "MyGameInstance.h"

void UWidgetHUDStageButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_OnMapChange.AddUObject(this, &UWidgetHUDStageButton::UpdateText);
}

void UWidgetHUDStageButton::UpdateText(EGameMode mode, int level)
{
	const FText& n = UMyGameInstance::Get->GetDefaultStageName(level);

	m_TextStageName->SetText(n);
}

UButton* UWidgetHUDStageButton::GetBtn()
{
	return m_BtnOpenStage;
}
