// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MyGameModeBase.h"

#include "Widgets/GameLevel/HUD/WidgetAlertPanel.h"

AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<UWidgetAlertPanel> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/MainGame/HUD/WB_AlertPanel.WB_AlertPanel_C'"));
	
	m_ClassCanvasAlert = FoundWW.Class;	
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();

	m_CanvasAlert = CreateWidget<UWidgetAlertPanel>(GetWorld(), m_ClassCanvasAlert);

	if(m_CanvasAlert)
	{
		m_CanvasAlert->AddToViewport(888);
	}
}

void AMyGameModeBase::ShowText(const FText& str, FLinearColor color)
{
	m_CanvasAlert->ShowText(str, color);
}
