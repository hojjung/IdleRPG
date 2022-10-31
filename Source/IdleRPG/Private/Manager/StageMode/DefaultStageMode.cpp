#include "Manager/StageMode/DefaultStageMode.h"

#include "IdleRPG.h"
#include "GAS/BigInt/BigIntCalcTableBase.h"
#include "Manager/MyGameInstance.h"
#include "Monsters/MonsterPawn.h"

UDataTable* UDefaultStageMode::GetStageData = nullptr;

TArray<const FStageRow*> UDefaultStageMode::AryStageRows = TArray<const FStageRow*>();

UDefaultStageMode::UDefaultStageMode()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/02_DataTables/StageTable.StageTable'"));

	GetStageData = FoundTable01.Object;

	AryStageRows.Reset(200);
	GetStageData->GetAllRows("", AryStageRows);
}

void UDefaultStageMode::OnPreSpawnMobs()
{
	Super::OnPreSpawnMobs();

	const FBigIntCalcTableRow* GoldRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultGold"), "");

	const FBigIntCalcTableRow* DmgRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultDmg"), "");

	const FBigIntCalcTableRow* HpRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultHp"), "");

	const FBigIntCalcTableRow* ExpRow = UBigIntCalcTableBase::GetBigIntTable->FindRow<FBigIntCalcTableRow>(TEXT("MobStatDefaultHp"), "");

	m_Gold = GoldRow->GetValue(m_nStageLevel);

	m_MobDmg = DmgRow->GetValue(m_nStageLevel);

	m_MobHp = HpRow->GetValue(m_nStageLevel);

	m_Exp = ExpRow->GetValue(m_nStageLevel);
	
	
}

TArray<FPrimaryAssetId> UDefaultStageMode::GetStageUnits(int lv)
{
	const FStageRow& StageRow = UDefaultStageMode::GetDefaultStage(lv);
	
	int Zone = lv % FGlobalVariable::STAGE_MAX;

	Zone /= FGlobalVariable::MOB_USECOUNT;

	TArray<FPrimaryAssetId> NewAry;
	
	NewAry.Add(StageRow.m_AryUnits[Zone]);

	int Iter = Zone;

	int Count = 0;

	while (--Iter >= 0 && Count < FGlobalVariable::MOB_PREUSECOUNT)
	{
		NewAry.Add(StageRow.m_AryUnits[Iter]);
		
		Count++;
	}
	
	return NewAry;
}

const FStageRow& UDefaultStageMode::GetDefaultStage(int stageLevel)
{
	int MapIndex = GetDefaultStageMapIndex(stageLevel); 

	return *UDefaultStageMode::AryStageRows[MapIndex];	
}

int UDefaultStageMode::GetDefaultStageMapIndex(int stageLevel)
{
	int Stage = stageLevel / FGlobalVariable::STAGE_MAX; 

	Stage = Stage % UDefaultStageMode::AryStageRows.Num();

	return Stage;
}

const FStageRow& UDefaultStageMode::GetDefaultStage()
{
	return GetDefaultStage(m_nStageLevel);
}

int UDefaultStageMode::GetDefaultStageMapIndex()
{
	return GetDefaultStageMapIndex(m_nStageLevel);
}

const FPrimaryAssetId& UDefaultStageMode::GetBossMonster(int stageLevel)
{
	return GetStageUnits(stageLevel)[0];
}

FText UDefaultStageMode::GetStageName(int lv)
{
	const FStageRow& StageWant = GetDefaultStage(lv);

	int Remain = lv % 20;

	FString Str = FString::Printf(TEXT("%s-%d"), *StageWant.m_StageName.ToString(), Remain + 1);

	return FText::FromString(Str);
}


void UDefaultStageMode::OnMonsterDead(AMonsterPawn* target)
{
	UMyGameInstance::Get->m_GoldManager->AddGold(m_Gold);
}

void UDefaultStageMode::OnMonsterAnimEnd(AMonsterPawn* target)
{
	FTimerHandle ReviveTimer;
	
	UMyGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(ReviveTimer, target, &AMonsterPawn::Revive, FMath::RandRange(4, 7), false);
}

void UDefaultStageMode::OnPlayerDead(AMyPlayerPawn* target)
{
	Super::OnPlayerDead(target);
}

void UDefaultStageMode::OnPlayerAnimEnd(AMyPlayerPawn* target)
{
	int PreLevel = FMath::Max(0, m_nStageLevel - FGlobalVariable::DEAD_PREV_STAGE);

	UMyGameInstance::Get->StartGameMode(EGameMode::Default, PreLevel);
}