#include "Manager/GameMode/DefaultGameMode.h"
#include "Monsters/MonsterPawn.h"
#include "Player/MyPlayerController.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

void ADefaultGameMode::StartPlay()
{
	AIdleRPGGameModeBase::StartPlay();

	int Lv = UMyGameInstance::Get->GetStageLevel();

	SetLevel(Lv);
}

void ADefaultGameMode::SetLevel(int l)
{
	Super::SetLevel(l);
	
	const FBigIntCalcTableRow* GoldRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultGold"), "");

	const FBigIntCalcTableRow* DmgRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultDmg"), "");

	const FBigIntCalcTableRow* HpRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultHp"), "");

	m_Gold = GoldRow->GetValue(m_nLevel);

	m_MobDmg = DmgRow->GetValue(m_nLevel);

	m_MobHp = HpRow->GetValue(m_nLevel);
	
	m_SpawnManager.Reset();
	
	m_SpawnManager = MakeShareable(new SpawnManager(m_MobHp, m_MobDmg));

	const FStageRow& StageRow = GetStage(m_nLevel);

	SpawnMobs();
}

void ADefaultGameMode::SpawnMobs()
{
	int Iter = -1;

	const FZone& Z = GetZone(m_nLevel);
	
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

const FStageRow& ADefaultGameMode::GetStage(int stageLevel) const
{
	int Stage = stageLevel / 20; 

	Stage = Stage % UStageTable::GetDefaultStage->GetRowMap().Num();

	return *UMyGameInstance::Get->GetDefaultStageRows()[Stage];
}

const FZone& ADefaultGameMode::GetZone(int stageLevel)//19, 0, 19
{
	int Zone = stageLevel % 20;

	const FZone& SelectZone = GetStage(stageLevel).m_AryUnits[Zone];

	return SelectZone;
}

const FPrimaryAssetId& ADefaultGameMode::GetBossMonster(int stageLevel)
{
	return GetZone(stageLevel).m_AryUnits[0];
}

FText ADefaultGameMode::GetStageName(int lv)
{
	const FStageRow& StageWant = GetStage(lv);

	int Remain = lv % 20;

	FString Str = FString::Printf(TEXT("%s-%d"), *StageWant.m_StageName.ToString(), Remain + 1);

	return FText::FromString(Str);
}

void ADefaultGameMode::OnPlayerDead()
{
	m_Canvas->OpenDeadAlert();
}
