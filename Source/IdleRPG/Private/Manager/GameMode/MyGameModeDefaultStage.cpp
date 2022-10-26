#include "Manager/GameMode/MyGameModeDefaultStage.h"
#include "GAS/BigInt/BigIntCalcTableBase.h"
#include "Manager/MyGameInstance.h"
#include "Monsters/StageTable.h"

MyGameModeDefaultStage::MyGameModeDefaultStage()
{
	
}

MyGameModeDefaultStage::~MyGameModeDefaultStage()
{
	
}



void MyGameModeDefaultStage::SetLevel(int l)
{
	MyGameModeBase::SetLevel(l);
	
	m_AryStage.Reserve(100);

	UStageTable::GetDefaultStage->GetAllRows<FStageRow>("", m_AryStage);

	const FBigIntCalcTableRow* GoldRow = UBigIntCalcTableBase::GetGoldTable->FindRow<FBigIntCalcTableRow>(TEXT("Default"), "");

	const FBigIntCalcTableRow* DmgRow = UBigIntCalcTableBase::GetMobDmgTable->FindRow<FBigIntCalcTableRow>(TEXT("Default"), "");

	const FBigIntCalcTableRow* HpRow = UBigIntCalcTableBase::GetMobHpTable->FindRow<FBigIntCalcTableRow>(TEXT("Default"), "");

	m_Gold = GoldRow->GetValue(m_nLevel);

	m_MobDmg = DmgRow->GetValue(m_nLevel);

	m_MobHp = HpRow->GetValue(m_nLevel);
	
	m_SpawnManager.Reset();
	
	m_SpawnManager = MakeShareable(new SpawnManager(m_MobHp, m_MobDmg));

	const FStageRow& StageRow = GetStage(m_nLevel);

	UMyGameInstance::Get->LoadMap(StageRow.m_LevelName, FVoidvoid::CreateRaw(this, &MyGameModeDefaultStage::SpawnMobs));
}

void MyGameModeDefaultStage::SpawnMobs()
{
	int Iter = -1;

	const FZone& Z = GetZone(m_nLevel);
	
	while (++Iter < 20)
	{
		const FPrimaryAssetId& SelectedId = m_SpawnManager->GetRandomMonsterID(Z); 
		
		m_SpawnManager->SpawnUnits(SelectedId);
	}
}

void MyGameModeDefaultStage::OnMonsterDead(AMonsterPawn* target)
{
	UMyGameInstance::Get->m_GoldManager->AddGold(m_Gold);
}

void MyGameModeDefaultStage::OnMonsterAnimEnd(AMonsterPawn* target)
{
	FTimerHandle ReviveTimer;
	
	UMyGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(ReviveTimer, target, &AMonsterPawn::Revive, FMath::RandRange(4, 7), false);
}

void MyGameModeDefaultStage::OnPlayerDead(AMyPlayerPawn* target)
{
	int PreLevel = FMath::Max(0, m_nLevel - 8);

	SetLevel(PreLevel);
}

const FStageRow& MyGameModeDefaultStage::GetStage(int stageLevel)
{
	int Stage = stageLevel % UStageTable::GetDefaultStage->GetRowMap().Num();

	return *m_AryStage[Stage];
}
const FZone& MyGameModeDefaultStage::GetZone(int stageLevel)//19, 0, 19
{
	int Zone = stageLevel % 20;

	const FZone& SelectZone = GetStage(stageLevel).m_AryUnits[Zone];

	return SelectZone;
}

const FPrimaryAssetId& MyGameModeDefaultStage::GetBossMonster(int stageLevel)
{
	return GetZone(stageLevel).m_AryUnits[0];
}

FText MyGameModeDefaultStage::GetStageName()
{
	const FStageRow& StageWant = GetStage(m_nLevel);
	
	float Index = m_nLevel / 20.0f;

	float UpIndex = 0.0f;

	float DownIndex = 0.0f;

	DownIndex = FMath::Modf(Index, &UpIndex);

	FString Str = FString::Printf(TEXT("%s-%d"), *StageWant.m_StageName.ToString(), (int)DownIndex + 1);

	return FText::FromString(Str);
}
