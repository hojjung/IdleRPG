
#include "Manager/StageMode/RaidStageMode.h"

#include "ConquerMap/ContentData.h"
#include "GAS/BigInt/BigIntCalcTableBase.h"
#include "Manager/MyGameInstance.h"
#include "Monsters/MonsterPawn.h"
#include "Widgets/GameLevel/WidgetRaidCanvas.h"

URaidStageMode::URaidStageMode()
{
	SetMobCount(1);

	static ConstructorHelpers::FClassFinder<UWidgetRaidCanvas> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/MainGame/MainMenu/WB_RaidCanvas.WB_RaidCanvas_C'"));
	
	m_ClassCanvas = FoundWW.Class;	
}

void URaidStageMode::TryAddModeWidget()
{
	Super::TryAddModeWidget();

	m_Canvas = CreateWidget<UWidgetRaidCanvas>(GetWorld(), m_ClassCanvas);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport(1);
	}
}

void URaidStageMode::OnPreSpawnMobs()
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

	SetMobCount(1);
}

TArray<FPrimaryAssetId> URaidStageMode::GetStageUnits(int lv)
{
	return m_Content->m_AryLevel[lv].m_AryUnits;
}

FText URaidStageMode::GetStageName(int lv)
{
	return m_Content->GetStageName(lv);
}

void URaidStageMode::SetContentData(const FContentDataRow& data)
{
	m_Content = &data;
}

void URaidStageMode::OnMonsterDead(AMonsterPawn* target)
{
	UMyGameInstance::Get->m_GoldManager->AddGold(m_Gold);
}

void URaidStageMode::OnMonsterDeadAnimEnd(AMonsterPawn* target)
{
	FTimerHandle ReviveTimer;
	
	UMyGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(ReviveTimer, target, &AMonsterPawn::Revive, FMath::RandRange(4, 7), false);
}

void URaidStageMode::OnPlayerDead(AMyPlayerPawn* target)
{
	Super::OnPlayerDead(target);
}

void URaidStageMode::OnPlayerDeadAnimEnd(AMyPlayerPawn* target)
{
	int DefaultLv = UMyGameInstance::Get->GetDefaultStageLevel();

	UMyGameInstance::Get->StartGameMode(DefaultLv, nullptr);
}

FVector URaidStageMode::GetSpawnLocation()
{
	return FVector(886, -824, 92);
}

FRotator URaidStageMode::GetSpawnRotation()
{
	return FRotator(0,90,0);
}