// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ColorData.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UColorData : public UObject
{
	GENERATED_BODY()

public:
	UColorData();
	
	static UDataTable* GetColorTable;
};

USTRUCT(BlueprintType)
struct FColorDataRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FSlateColor m_Color;
	UPROPERTY(EditDefaultsOnly)
	FText m_Name;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* m_GlowTexture;
	UPROPERTY(EditDefaultsOnly)
	float m_fPriority = 1;
};

USTRUCT(BlueprintType)
struct FColorDataHandle : public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()

public:
	FColorDataHandle()
	{
		DataTable = UColorData::GetColorTable;
		RowName = TEXT("Default");
	}
};