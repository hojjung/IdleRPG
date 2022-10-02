#include "Monsters/SpawnManager.h"

#include "Entity.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "MyLib.h"
#include "NavigationSystem.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Monsters/MonsterPawn.h"
#include "Monsters/StageTable.h"
#include "Player/MyPlayerPawn.h"

SpawnManager::SpawnManager()
{
	m_AryStage.Reserve(100);
	UStageTable::GetData->GetAllRows<FStageRow>("", m_AryStage);
}

SpawnManager::~SpawnManager()
{
	for(auto Mob : m_AryMonsters)
	{
		Mob.Reset();
	}
	m_AryMonsters.Reset();

	m_QuadTree.Reset();
}

const FPrimaryAssetId& SpawnManager::GetRandomMonsterID(int stageLevel)
{
	int MaxIndex = m_AryStage[stageLevel]->m_AryUnits.Num() - 1;

	int RandIndex = FMath::RandRange(0, MaxIndex);

	return m_AryStage[stageLevel]->m_AryUnits[RandIndex];
}

void SpawnManager::Update(float delta)
{
	if(m_AryMonsters.Num() <= 0)
	{
		return;
	}
	for(auto Mob : m_AryMonsters)
	{
		Mob.Get()->Update(delta);
	}
	m_QuadTree->UpdateState(UMyGameInstance::Get);
}

void SpawnManager::SpawnUnits(const UObject* world, int stageLevel, ACombatPawn::FOnDied dele,  int cnt)
{
	if(m_AryStage[stageLevel]->m_AryUnits.Num() <= 0)
	{
		PRINTF("SpawnManager Spawn Unit Fail, No Units in DataTable");
		return;
	}
	
	const UNavigationSystemV1* Nav =  FNavigationSystem::GetCurrent<UNavigationSystemV1>(world->GetWorld());

	FBox NavBox = Nav->GetNavigationBounds().Array()[0].AreaBox;
	
	m_QuadTree = MakeShareable(new QuadTree(NavBox.GetCenter(), NavBox.GetExtent() * 1.3f,  4));
	m_QuadTree->m_Root = m_QuadTree;

	int Iter = -1;
	
	while (++Iter < cnt)
	{
		FNavLocation ResultPos;
	
		Nav->GetRandomPointInNavigableRadius(NavBox.GetCenter(), NavBox.GetExtent().X, ResultPos);

		FRotator Rot = FRotator(0,FMath::RandRange(0, 360),0);
		
		const FPrimaryAssetId& AssetID = GetRandomMonsterID(stageLevel);

		FStreamableDelegate Delegate = FStreamableDelegate::CreateRaw(this, &SpawnManager::OnMonsterLoaded, AssetID, world, ResultPos.Location, Rot, dele);

		UMyAssetManager::Get()->LoadUnitAssetMeshOnly(AssetID, Delegate);
	}
}

void SpawnManager::Clear()
{
	UMyGameInstance::Get->m_Player->SetFocusedTarget(nullptr);

	for(TSharedPtr<Monster, ESPMode::NotThreadSafe> Mob : m_AryMonsters)
	{
		Mob.Get()->GetMonsterPawn()->Destroy();
		Mob.Reset();
	}
	m_AryMonsters.Reset();
}

void SpawnManager::OnMonsterLoaded(const FPrimaryAssetId id, const UObject* world, FVector loc, FRotator rot,  ACombatPawn::FOnDied dele)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Param.bNoFail = true;
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	
	UUnitAsset* MonsterData = Cast<UUnitAsset>(Manager->GetPrimaryAssetObject(id));
	
	AMonsterPawn* Pawn = world->GetWorld()->SpawnActor<AMonsterPawn>(AMonsterPawn::StaticClass(),loc, rot, Param);

	Pawn->m_OnDied = dele;
	
	m_QuadTree->InsertObject(Pawn);

	Pawn->SetEntity(MonsterData);

	TSharedPtr<Monster> Mob = MakeShareable(new Monster(Pawn));
		
	Pawn->SetMonsterInst(Mob);

	auto* Play = UMyLib::GetPlayer();
	
	Pawn->SetFocusedTarget(Play);

	m_AryMonsters.Add(Mob);
}
