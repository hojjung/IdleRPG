// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/EntityData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "SkillData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillData : public UObject
{
	GENERATED_BODY()
public:
	USkillData();

	static UDataTable* GetSkillData;
};
USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FSkillDataRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()
public://buff actice passvie
};