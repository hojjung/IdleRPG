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

	static UDataTable* GetData;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FStageRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UUnitAsset>> m_AryUnits;
};