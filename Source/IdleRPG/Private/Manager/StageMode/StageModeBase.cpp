// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/StageMode/StageModeBase.h"

#include "Manager/MyGameInstance.h"

void UStageModeBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void UStageModeBase::SetLevel(int l)
{
	m_nStageLevel = l;

	OnPreSpawnMobs();

	SpawnMobs();
}

void UStageModeBase::OnPreSpawnMobs()
{
	
}

void UStageModeBase::SpawnMobs()
{
	int Iter = -1;

	TArray<FPrimaryAssetId> Z = GetStageUnits(m_nStageLevel);
	
	while (++Iter < 20)
	{
		const FPrimaryAssetId& SelectedId = m_SpawnManager->GetRandomMonsterID(Z); 
		
		m_SpawnManager->SpawnUnits(SelectedId);
	}
}

TArray<FPrimaryAssetId> UStageModeBase::GetStageUnits(int lv)
{
	return TArray<FPrimaryAssetId>(); 
}

FText UStageModeBase::GetStageName()
{
	return GetStageName(m_nStageLevel);
}

FText UStageModeBase::GetStageName(int level)
{
	return FText();
}

void UStageModeBase::OnMonsterDead(AMonsterPawn* target)
{
	UMyGameInstance::Get->m_GoldManager->AddGold(m_Gold);
	UMyGameInstance::Get->m_GoldManager->AddGold(m_Exp);
}

void UStageModeBase::Tick(float d)
{
	m_SpawnManager->Update(d);
}
