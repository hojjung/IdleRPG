// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/UnitAsset.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "StageTable.generated.h"

/**
 * DataTable'/Game/DataTables/StageTable.StageTable'
 */

UCLASS()
class IDLERPG_API UStageTable : public UObject
{
	GENERATED_BODY()

public:
	UStageTable();

	static UDataTable* GetDefaultStage;
};

USTRUCT(BlueprintType)
struct FZone
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPrimaryAssetId> m_AryUnits;
	//스테이지 추가 드랍
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
	TArray<FZone> m_AryUnits;
	//클리어보상
};
