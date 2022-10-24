// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameModeBase.h"

struct FZone;
struct FStageRow;
struct FBigIntCalcTableRow;
/**
 * 기본 스테이지
 */
class IDLERPG_API MyGameModeDefaultStage : public  MyGameModeBase
{
public:
	MyGameModeDefaultStage(int lv);
	
	virtual ~MyGameModeDefaultStage() override;

protected:
	TArray<FStageRow*> m_AryStage;
	
	BigInt m_Gold;

private:
	const FStageRow& GetStage(int stageLevel);

	const FZone& GetZone(int stageLevel);

	const FPrimaryAssetId& GetBossMonster(int stageLevel);

	virtual FText GetDefaultStageName(int level) override;

public:
	virtual void OnMonsterDead(AMonsterPawn* target) override;
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) override;

	virtual void OnPlayerDead(AMyPlayerPawn* target) override;
};
