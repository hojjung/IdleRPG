// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColorData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "EntityData.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UEntityData : public UObject
{
	GENERATED_BODY()
public:
	UEntityData();
	
	static UDataTable* GetColorTable;
};


USTRUCT(BlueprintType)
struct FEntityDataRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText m_Name;
	UPROPERTY(EditDefaultsOnly)
	FColorDataHandle m_Color;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_Icon;
	UPROPERTY(EditDefaultsOnly)
	float m_fPriority = 1;

public:
	FORCEINLINE const FColorDataRow& GetColor() const
	{
		const FColorDataRow* ColorData = m_Color.GetRow<FColorDataRow>("");

		return  *ColorData;
	}
};