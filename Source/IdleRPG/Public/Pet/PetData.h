// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/EntityData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "PetData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UPetData : public UObject
{
	GENERATED_BODY()
public:
	UPetData();

	static UDataTable* GetPetData;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FPetDataRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId m_EntityAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nTier = 0;
};