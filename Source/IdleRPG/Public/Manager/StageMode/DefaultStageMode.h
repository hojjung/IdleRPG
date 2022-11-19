// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Manager/StageMode/StageModeBase.h"
#include "DefaultStageMode.generated.h"

class UWidgetDefaultCanvas;
struct FStageRow;
/**
 * 
 */
UCLASS()
class IDLERPG_API UDefaultStageMode : public UStageModeBase
{
	GENERATED_BODY()

public:
	UDefaultStageMode();
	
protected:
	static UDataTable* GetStageData;

	TSubclassOf<UWidgetDefaultCanvas> m_ClassCanvas;
	UPROPERTY()
	UWidgetDefaultCanvas* m_Canvas;
public:
	static TArray<const FStageRow*> AryStageRows;

protected:
	virtual void OnPreSpawnMobs() override;

	virtual TArray<FPrimaryAssetId> GetStageUnits(int lv) override;

	virtual void TryAddModeWidget() override;
	
public:
	virtual FText GetStageName(int level) override;
	
	static const FStageRow& GetDefaultStage(int stageLevel);

	static int GetDefaultStageMapIndex(int stageLevel);

	const FStageRow& GetDefaultStage();
	
	int GetDefaultStageMapIndex();

	const FPrimaryAssetId& GetBossMonster(int stageLevel);

public:
	virtual void OnMonsterDead(AMonsterPawn* target) override;

	virtual void OnMonsterDeadAnimEnd(AMonsterPawn* target) override;

	virtual void OnPlayerDead(AMyPlayerPawn* target) override;

	virtual void OnPlayerDeadAnimEnd(AMyPlayerPawn* target) override;
};




USTRUCT(BlueprintType)//일반 스테이지
struct FStageRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_LevelName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_StageName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_IconStage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPrimaryAssetId> m_AryUnits;//10개
};
