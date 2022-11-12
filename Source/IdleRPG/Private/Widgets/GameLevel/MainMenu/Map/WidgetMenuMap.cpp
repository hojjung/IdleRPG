// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetMenuMap.h"

#include "Widgets/Lib/WidgetLib.h"

void UWidgetMenuMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_BtnStory->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickStory);
	m_BtnMine->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickMine);
	m_BtnPVP->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickColosseum);
	m_BtnElf->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickElf);
	m_BtnJapan->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickJapan);
	m_BtnBoneDragon->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickRaid01);
	m_BtnReaper->OnClicked.AddDynamic(this, &UWidgetMenuMap::OnClickRaid02);

	UWidgetLib::SetCurrentButton(m_Current, m_BtnStory);

	m_Map->SetOnClickEvent(UWidgetMapEle::FOnClick::CreateUObject(this, &UWidgetMenuMap::OnClickBtn));
}

void UWidgetMenuMap::OnClickStory()
{
	m_Map->MoveToStoryUnlock(0);
	UWidgetLib::SetCurrentButton(m_Current, m_BtnStory);
}

void UWidgetMenuMap::OnClickJapan()
{
	m_Map->MoveToContent(TEXT("Japan"));
	UWidgetLib::SetCurrentButton(m_Current, m_BtnJapan);
}

void UWidgetMenuMap::OnClickColosseum()
{
	m_Map->MoveToContent(TEXT("PVP"));
	UWidgetLib::SetCurrentButton(m_Current, m_BtnPVP);
}

void UWidgetMenuMap::OnClickMine()
{
	m_Map->MoveToContent(TEXT("Mine"));
	UWidgetLib::SetCurrentButton(m_Current, m_BtnMine);
}

void UWidgetMenuMap::OnClickElf()
{
	m_Map->MoveToContent(TEXT("Elf"));
	UWidgetLib::SetCurrentButton(m_Current, m_BtnElf);
}

void UWidgetMenuMap::OnClickRaid01()
{
	m_Map->MoveToContent(TEXT("Raid01"));
	UWidgetLib::SetCurrentButton(m_Current, m_BtnBoneDragon);
}

void UWidgetMenuMap::OnClickRaid02()
{
	m_Map->MoveToContent(TEXT("Raid02"));
	UWidgetLib::SetCurrentButton(m_Current, m_BtnReaper);
}

void UWidgetMenuMap::OnClickBtn(const FName& mapZoneID)
{
	m_Select->Open(mapZoneID);
}