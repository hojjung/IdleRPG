// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/StageMode/StageModeBase.h"
#include "RaidStageMode.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API URaidStageMode : public UStageModeBase
{
	GENERATED_BODY()

public:
	URaidStageMode();

protected:
	const FContentDataRow* m_Content;
	
protected:
	virtual void OnPreSpawnMobs() override;

	virtual TArray<FPrimaryAssetId> GetStageUnits(int lv) override;
	
public:
	virtual FText GetStageName(int level) override;

	void SetContentData(const FContentDataRow& data);
	
public:
	virtual void OnMonsterDead(AMonsterPawn* target) override;

	virtual void OnMonsterDeadAnimEnd(AMonsterPawn* target) override;

	virtual void OnPlayerDead(AMyPlayerPawn* target) override;

	virtual void OnPlayerDeadAnimEnd(AMyPlayerPawn* target) override;

	
};

