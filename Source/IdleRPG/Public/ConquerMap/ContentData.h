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

USTRUCT(BlueprintType)
struct FContentMobData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPrimaryAssetId> m_AryUnits;//10개
};
UENUM(BlueprintType)
enum class EGameMode : uint8
{
	PVP,
	Mine,
	Elf,
	Japan,
	BoneDragon,
	Reaper,
	Castle,
	Length
};
USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FContentDataRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_ConentIcon;
	UPROPERTY(EditDefaultsOnly)
	FText m_Desc;
	UPROPERTY(EditDefaultsOnly)
	FName m_MapName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FContentMobData> m_AryLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGameMode m_GameMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nDailyMax = -1;

public:
	FORCEINLINE FText GetStageName(int lv) const
	{
		FString NameStr = m_Name.ToString();
	
		return FText::FromString(FString::Printf(TEXT("%s - %d"), *NameStr, lv));
	}
};