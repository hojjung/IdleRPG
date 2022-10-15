// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Acce/WidgetAcceMenu.h"

#include "MyGameInstance.h"

void UWidgetAcceMenu::OnOpen()
{
	Super::OnOpen();

	if(UMyGameInstance::Get->GetPlayerPawn())
	{
		UMyGameInstance::Get->GetPlayerPawn()->SetCameraTop();
	}
}

void UWidgetAcceMenu::OnClose()
{
	Super::OnClose();

	if(UMyGameInstance::Get->GetPlayerPawn())
	{
		UMyGameInstance::Get->GetPlayerPawn()->SetCameraCenter();
	}
}
