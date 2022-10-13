// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"

#include "MyGameInstance.h"

void UWidgetStageEle::SetZone(const FText& z, int level)
{
	m_nLevel = level;

	FString Str = FString::Printf(TEXT("%s-%d"), *z.ToString(), m_nLevel + 1); 

	m_TextName->SetText(FText::FromString(Str));

	m_BtnEnter->OnClicked.AddDynamic(this, &UWidgetStageEle::OnEnter);
}

void UWidgetStageEle::OnEnter()
{
	UMyGameInstance::Get->StartGameMode(EGameMode::Default, m_nLevel);
}
