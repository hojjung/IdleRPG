// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/ColorData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "EquipData.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UEquipData : public UObject
{
	GENERATED_BODY()
	
public:
	UEquipData();

	static UDataTable* GetWeaponData;

	static UDataTable* GetHelmetData;

	static UDataTable* GetTorsoData;

	static UDataTable* GetGloveData;

	static UDataTable* GetLegData;

	static UDataTable* GetFeetData;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FEquipRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nTier = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColorDataHandle m_Color;
};