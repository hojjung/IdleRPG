// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "EmblemData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UEmblemData : public UObject
{
	GENERATED_BODY()
public:
	UEmblemData();
	
	static UDataTable* GetUpgradeTable;
};
USTRUCT(BlueprintType)
struct FEmblemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_StrUpgradeDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxLevel;
};
