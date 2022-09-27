// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "UpgradeData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UUpgradeData : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FUpgradeDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString m_StrUpgradeDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxLevel;
};
