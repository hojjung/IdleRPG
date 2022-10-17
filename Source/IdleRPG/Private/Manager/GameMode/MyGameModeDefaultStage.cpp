#include "Manager/GameMode/MyGameModeDefaultStage.h"

#include "MyGameInstance.h"
#include "GAS/CalcLib.h"

MyGameModeDefaultStage::MyGameModeDefaultStage()
{
}

MyGameModeDefaultStage::~MyGameModeDefaultStage()
{
}

void MyGameModeDefaultStage::OnMonsterDead(AMonsterPawn* target)
{
	int Lv = UMyGameInstance::Get->GetStageLevel();

	BigInt Gold = UCalcLib::GetGoldPerLevel(Lv);

	UMyGameInstance::Get->m_GoldManager->AddGold(Gold);
}

void MyGameModeDefaultStage::OnMonsterAnimEnd(AMonsterPawn* target)
{
	FTimerHandle m_ReviveTimer;
	
	UMyGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_ReviveTimer, target, &AMonsterPawn::Revive, FMath::RandRange(4, 7), false);
}
