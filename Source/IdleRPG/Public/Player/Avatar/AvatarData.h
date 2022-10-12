// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/EntityData.h"
#include "Engine/DataTable.h"
#include "GameAbilitySystem/MyStat.h"
#include "AvatarData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UAvatarData : public UObject
{
	GENERATED_BODY()

public:
	UAvatarData();
	
	static UDataTable* GetAvatarTable;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FAvatarRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId m_EntityAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FStatGroup m_BonusStat;
};