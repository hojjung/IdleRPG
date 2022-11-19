#include "Monsters/SpawnManager.h"
#include "Entity.h"
#include "MyAssetManager.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Monsters/MonsterPawn.h"
#include "Player/MyPlayerPawn.h"
#include "Manager/MyGameInstance.h"

SpawnManager::SpawnManager()
{
	m_Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UMyGameInstance::Get->GetWorld());

	m_NavBox = m_Nav->GetNavigationBounds().Array()[0].AreaBox;

	m_QuadTree = MakeShareable(new QuadTree(m_NavBox.GetCenter(), m_NavBox.GetExtent() * 1.3f, 4));

	m_QuadTree->m_Root = m_QuadTree;
}

SpawnManager::~SpawnManager()
{
	for (auto Mob : m_AryMonsters)
	{
		AMonsterPawn* m = Mob.Get()->GetMonsterPawn();
		if(m != nullptr)
		{
			m->Destroy();
		}
		Mob.Reset();
	}
	m_AryMonsters.Reset();

	m_QuadTree.Reset();
}

void SpawnManager::Update(float delta)
{
	for (auto Mob : m_AryMonsters)
	{
		Mob.Get()->Update(delta);
	}
	m_QuadTree->UpdateState(UMyGameInstance::Get);
}

const FPrimaryAssetId& SpawnManager::GetRandomMonsterID(const TArray<FPrimaryAssetId>& z)
{
	int RandIndex = FMath::RandRange(0, z.Num() - 1);

	return z[RandIndex];
}

void SpawnManager::SpawnUnits(const FPrimaryAssetId& id, FVector loc, FRotator rot) 
{
	const FPrimaryAssetId& AssetID = id;

	const UObject* Inst = UMyGameInstance::Get;

	FStreamableDelegate Delegate = FStreamableDelegate::CreateRaw(this, &SpawnManager::OnMonsterLoaded, AssetID, Inst, loc, rot);

	UMyAssetManager::Get()->LoadUnitAssetMeshOnly(AssetID, Delegate);
}

void SpawnManager::OnMonsterLoaded(const FPrimaryAssetId id, const UObject* world, FVector loc, FRotator rot)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Param.bNoFail = true;

	UAssetManager* Manager = UAssetManager::GetIfValid();

	UUnitAsset* MonsterData = Cast<UUnitAsset>(Manager->GetPrimaryAssetObject(id));

	loc.Z += 88;

	AMonsterPawn* Pawn = world->GetWorld()->SpawnActor<AMonsterPawn>(AMonsterPawn::StaticClass(), loc, rot, Param);

	m_QuadTree->InsertObject(Pawn);

	Pawn->SetEntity(MonsterData);

	TSharedPtr<Monster> Mob = MakeShareable(new Monster(Pawn));

	Pawn->GetGas()->SetDefaultStat(m_Hp, m_Dmg);

	m_AryMonsters.Add(Mob);
}

void SpawnManager::SetBigIntStagMob(BigInt hp, BigInt dmg)
{
	m_Hp = hp;
	
	m_Dmg = dmg;
}
