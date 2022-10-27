// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IdleRPGGameModeBase.h"

struct FZone;
struct FStageRow;
struct FBigIntCalcTableRow;
/**
 * 기본 스테이지
 */
class IDLERPG_API MyGameModeDefaultStage : public  AIdleRPGGameModeBase
{
protected:
	BigInt m_Gold;

private:
	virtual void StartPlay() override;
	
	virtual void SetLevel(int l) override;
	
	const FStageRow& GetStage(int stageLevel) const;

	const FZone& GetZone(int stageLevel);

	const FPrimaryAssetId& GetBossMonster(int stageLevel);

	virtual FText GetStageName() override;

public:
	void SpawnMobs();
	
	virtual void OnMonsterDead(AMonsterPawn* target) override;
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) override;

	virtual void OnPlayerDead(AMyPlayerPawn* target) override;
};
