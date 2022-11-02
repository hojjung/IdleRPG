// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetMenuMap.h"

void UWidgetMenuMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnStory->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickStory);
	m_BtnMine->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickMine);
	m_BtnVillage->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickVillage);
	m_BtnPVP->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickColosseum);
	m_BtnElf->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickElf);
	m_BtnJapan->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickJapan);
	m_BtnBoneDragon->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickRaid01);
	m_BtnReaper->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickRaid02);
}

void UWidgetMenuMap::OnClickStory()
{
	m_Map->MoveToStoryUnlock(0);
}

void UWidgetMenuMap::OnClickJapan()
{
	m_Map->MoveToContent(TEXT("Japan"));
}

void UWidgetMenuMap::OnClickVillage()
{
	m_Map->MoveToContent(TEXT("Village"));
}

void UWidgetMenuMap::OnClickColosseum()
{
	m_Map->MoveToContent(TEXT("PVP"));
}

void UWidgetMenuMap::OnClickMine()
{
	m_Map->MoveToContent(TEXT("Mine"));
}

void UWidgetMenuMap::OnClickElf()
{
	m_Map->MoveToContent(TEXT("Elf"));
}

void UWidgetMenuMap::OnClickRaid01()
{
	m_Map->MoveToContent(TEXT("Raid01"));
}

void UWidgetMenuMap::OnClickRaid02()
{
	m_Map->MoveToContent(TEXT("Raid02"));
}
