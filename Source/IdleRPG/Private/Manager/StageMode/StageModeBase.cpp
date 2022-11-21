// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/StageMode/StageModeBase.h"

#include "NavigationSystem.h"
#include "Manager/MyGameInstance.h"

UStageModeBase::UStageModeBase()
{
	m_nMobSpawnCount = 20;
}

void UStageModeBase::SetLevel(int l)
{
	m_nStageLevel = l;
}

void UStageModeBase::SetMobCount(int cnt)
{
	m_nMobSpawnCount = cnt;
}

void UStageModeBase::StartGame()
{
	OnPreSpawnMobs();

	SpawnMobs();
}

void UStageModeBase::OnPreSpawnMobs()
{
	
}

void UStageModeBase::SpawnMobs()
{
	m_SpawnManager.Reset();
	
	m_SpawnManager = MakeShareable(new SpawnManager());

	m_SpawnManager->SetBigIntStagMob(m_MobHp, m_MobDmg);
	int Iter = -1;

	TArray<FPrimaryAssetId> Z = GetStageUnits(m_nStageLevel);
	
	while (++Iter < m_nMobSpawnCount)
	{
		const FPrimaryAssetId& SelectedId = m_SpawnManager->GetRandomMonsterID(Z);

		FVector Loc = GetSpawnLocation();

		FRotator Rot = GetSpawnRotation();

		m_SpawnManager->SpawnUnits(SelectedId, Loc, Rot);
	}
}

FVector UStageModeBase::GetSpawnLocation()
{
	FNavLocation ResultPos;

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UMyGameInstance::Get->GetWorld());

	FBox NavBox = Nav->GetNavigationBounds().Array()[0].AreaBox;

	Nav->GetRandomReachablePointInRadius(NavBox.GetCenter(), NavBox.GetExtent().X, ResultPos);

	return ResultPos.Location;
}

FRotator UStageModeBase::GetSpawnRotation()
{
	FRotator Rot = FRotator(0, FMath::RandRange(0, 360), 0);
	
	return Rot; 
}

void UStageModeBase::Clear()
{
	m_SpawnManager.Reset();	
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
	UMyGameInstance::Get->m_ExpManager->AddExp(m_Exp);
}

void UStageModeBase::OnPlayerDead(AMyPlayerPawn* target)
{
	UMyGameInstance::Get->m_bIsPlayerDead = true;
	UMyGameInstance::Get->GetGameModeActor()->SetFade();
}

void UStageModeBase::Tick(float d)
{
	m_SpawnManager->Update(d);
}
