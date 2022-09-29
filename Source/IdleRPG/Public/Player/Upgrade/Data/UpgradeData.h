// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/ColorData.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "UpgradeData.generated.h"

/**
 * DataTable'/Game/02_DataTables/UpgradeTable.UpgradeTable'
 */
UCLASS()
class IDLERPG_API UUpgradeData : public UObject
{
	GENERATED_BODY()

public:
	UUpgradeData();
	
	static UDataTable* GetUpgradeTable;
};

USTRUCT(BlueprintType)
struct FUpgradeDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColorDataHandle m_Color;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_StrUpgradeDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxLevel;
};
