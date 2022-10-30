#include "Manager/GameMode/DefaultGameMode.h"
#include "Monsters/MonsterPawn.h"
#include "Player/MyPlayerController.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

void ADefaultGameMode::SetLevel(int l)
{
	Super::SetLevel(l);
	
	const FBigIntCalcTableRow* GoldRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultGold"), "");

	const FBigIntCalcTableRow* DmgRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultDmg"), "");

	const FBigIntCalcTableRow* HpRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultHp"), "");

	m_Gold = GoldRow->GetValue(m_nLevel);

	m_MobDmg = DmgRow->GetValue(m_nLevel);

	m_MobHp = HpRow->GetValue(m_nLevel);
	
	m_SpawnManager->SetBigIntStagMob(m_MobHp, m_MobDmg);

	SpawnMobs();
}

void ADefaultGameMode::SpawnMobs()
{
	int Iter = -1;

	TArray<FPrimaryAssetId> Z = GetZone(m_nLevel);
	
	while (++Iter < 20)
	{
		const FPrimaryAssetId& SelectedId = m_SpawnManager->GetRandomMonsterID(Z); 
		
		m_SpawnManager->SpawnUnits(SelectedId);
	}
}

void ADefaultGameMode::OnMonsterDead(AMonsterPawn* target)
{
	UMyGameInstance::Get->m_GoldManager->AddGold(m_Gold);
}

void ADefaultGameMode::OnMonsterAnimEnd(AMonsterPawn* target)
{
	FTimerHandle ReviveTimer;
	
	UMyGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(ReviveTimer, target, &AMonsterPawn::Revive, FMath::RandRange(4, 7), false);
}

void ADefaultGameMode::OnPlayerDead(AMyPlayerPawn* target)
{
	
}

void ADefaultGameMode::OnPlayerAnimEnd(AMyPlayerPawn* target)
{
	int PreLevel = FMath::Max(0, m_nLevel - 8);

	FVoidvoid OnDead = FVoidvoid::CreateUObject(this, &ADefaultGameMode::OnPlayerDead);
	
	UMyGameInstance::Get->StartGameMode(EGameMode::Default, PreLevel, OnDead);
}

const FStageRow& ADefaultGameMode::GetDefaultStage(int stageLevel)
{
	int MapIndex = GetDefaultStageMapIndex(stageLevel); 

	return *UMyGameInstance::Get->GetDefaultStageRows()[MapIndex];
}

int ADefaultGameMode::GetDefaultStageMapIndex(int stageLevel)
{
	int Stage = stageLevel / 20; 

	Stage = Stage % UStageTable::GetDefaultStage->GetRowMap().Num();

	return Stage;
}

TArray<FPrimaryAssetId> ADefaultGameMode::GetZone(int stageLevel)//19, 0, 19
{
	TArray<FPrimaryAssetId> SelectZone = GetDefaultStage(stageLevel).GetStageUnits(stageLevel);

	return SelectZone;
}

const FPrimaryAssetId& ADefaultGameMode::GetBossMonster(int stageLevel)
{
	return GetZone(stageLevel)[0];
}

FText ADefaultGameMode::GetStageName(int lv)
{
	const FStageRow& StageWant = GetDefaultStage(lv);

	int Remain = lv % 20;

	FString Str = FString::Printf(TEXT("%s-%d"), *StageWant.m_StageName.ToString(), Remain + 1);

	return FText::FromString(Str);
}

void ADefaultGameMode::OnPlayerDead()
{
	m_Canvas->OpenDeadAlert();
}
