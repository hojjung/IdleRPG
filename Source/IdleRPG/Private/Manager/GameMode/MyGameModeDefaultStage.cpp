#include "Manager/GameMode/MyGameModeDefaultStage.h"
#include "MyGameInstance.h"
#include "GAS/BigInt/BigIntCalcTableBase.h"

MyGameModeDefaultStage::MyGameModeDefaultStage()
{
	
}

MyGameModeDefaultStage::~MyGameModeDefaultStage()
{
}

void MyGameModeDefaultStage::OnMonsterDead(AMonsterPawn* target)
{
	int Lv = UMyGameInstance::Get->GetStageLevel();

	UMyGameInstance::Get->m_GoldManager->AddGold(m_Gold);
}

void MyGameModeDefaultStage::OnMonsterAnimEnd(AMonsterPawn* target)
{
	FTimerHandle m_ReviveTimer;
	
	UMyGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_ReviveTimer, target, &AMonsterPawn::Revive, FMath::RandRange(4, 7), false);
}

void MyGameModeDefaultStage::SetLevel(int lv)
{
	MyGameModeBase::SetLevel(lv);

	const FBigIntCalcTableRow* GoldRow = UBigIntCalcTableBase::GetGoldTable->FindRow<FBigIntCalcTableRow>(TEXT("Default"), "");

	const FBigIntCalcTableRow* DmgRow = UBigIntCalcTableBase::GetMobDmgTable->FindRow<FBigIntCalcTableRow>(TEXT("Default"), "");

	const FBigIntCalcTableRow* HpRow = UBigIntCalcTableBase::GetMobHpTable->FindRow<FBigIntCalcTableRow>(TEXT("Default"), "");

	m_Gold = GoldRow->GetValue(m_nLevel);

	m_MobDmg = DmgRow->GetValue(m_nLevel);

	m_MobHp = HpRow->GetValue(m_nLevel);
}