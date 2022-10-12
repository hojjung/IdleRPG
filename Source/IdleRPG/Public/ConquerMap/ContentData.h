// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/EntityData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ContentData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UContentData : public UObject
{
	GENERATED_BODY()
public:
	UContentData();

	static UDataTable* GetContentData;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FContentDataRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()
};