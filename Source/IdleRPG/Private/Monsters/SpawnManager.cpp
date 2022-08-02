#include "Monsters/SpawnManager.h"

#include "MyAssetManager.h"
#include "MyGameInstance.h"
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

void SpawnManager::Update(float delta)
{
	for(auto Mob : m_AryMonsters)
	{
		Mob.Get()->Update(delta);
	}
	m_QuadTree->UpdateState(UMyGameInstance::Get);
}

void SpawnManager::SpawnUnits(UObject* world, int stageLevel, int cnt)
{
	check(m_AryStage[stageLevel]->m_AryUnits.Num() > 0);
	
	UNavigationSystemV1* Nav =  FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);

	FBox NavBox = Nav->GetNavigationBounds().Array()[0].AreaBox;
	
	m_QuadTree = MakeShareable(new QuadTree(NavBox.GetCenter(), NavBox.GetExtent(),  4));
	m_QuadTree->m_Root = m_QuadTree;
	
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Param.bNoFail = true;

	int Iter = -1;
	
	while (++Iter < cnt)
	{
		FNavLocation ResultPos;
	
		Nav->GetRandomPointInNavigableRadius(NavBox.GetCenter(), NavBox.GetExtent().X, ResultPos);

		FRotator Rot = FRotator(0,FMath::RandRange(0, 360),0);
		
		AMonsterPawn* Pawn = world->GetWorld()->SpawnActor<AMonsterPawn>(AMonsterPawn::StaticClass(),ResultPos, Rot, Param);

		auto ASD = Cast<AMyPlayerPawn>( UGameplayStatics::GetPlayerPawn(world, 0));
		
		Pawn->SetFocusedTarget(ASD);

		m_QuadTree->InsertObject(Pawn);
		
		int MaxIndex = m_AryStage[stageLevel]->m_AryUnits.Num() - 1;

		int RandIndex = FMath::RandRange(0, MaxIndex);

		TSoftObjectPtr<UUnitEntityAsset> UnitSoft = m_AryStage[stageLevel]->m_AryUnits[RandIndex];

		UUnitEntityAsset* LoadedUnit = UMyAssetManager::Get()->LoadUnitAsset(UnitSoft);

		Pawn->SetEntity(LoadedUnit);

		TSharedPtr<Monster> Mob = MakeShareable(new Monster(Pawn, LoadedUnit));
		
		Pawn->SetMonsterInst(Mob);

		m_AryMonsters.Add(Mob);
	}
}