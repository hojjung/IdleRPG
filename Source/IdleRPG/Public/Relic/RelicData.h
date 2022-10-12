// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/ColorData.h"
#include "DataTableRow/EntityData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "RelicData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API URelicData : public UObject
{
	GENERATED_BODY()
public:
	URelicData();

	static UDataTable* GetRelicData;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FRelicRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nTier = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxLevel = 100;
};