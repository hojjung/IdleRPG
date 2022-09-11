// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/ColorData.h"
#include "Engine/DataTable.h"
#include "GameAbilitySystem/MyStat.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORY_API UItemData : public UObject
{
	GENERATED_BODY()

public:
	UItemData();
	
	static UDataTable* GetItemTable;
};


USTRUCT(BlueprintType)
struct FItemDataHandle : public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()

public:
	FItemDataHandle()
	{
		DataTable = UItemData::GetItemTable;
	}

	TSoftObjectPtr<UTexture2D> GetItemIcon() const;
};


USTRUCT(BlueprintType)//플레이어는 처음부터 무엇을 제작할수 있는지는 전부 확인 가능하다
struct FCraftItemCost
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDataHandle m_ItemDataRowHandle;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nStackOrLevel=1;
};


USTRUCT(BlueprintType)
struct FDropData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_ZoneUniqueID;
	UPROPERTY(EditDefaultsOnly)
	int m_nExpectDropCount=5;//5번에 한번,20% 드랍이란뜻임
};
USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FEntityRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine="true"))
	FText m_Desc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_Icon;

	virtual UStaticMesh* GetStMesh()
	{
		return nullptr;
	}
	virtual USkeletalMesh* GetSkMesh()
	{
		return nullptr;
	}
	virtual float GetMeshScale()
	{
		return 1.f;
	}
	
};
USTRUCT(BlueprintType)
struct FCraftable :  public FEntityRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCraftSortOrder=0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCraftLevelLimit=1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FCraftItemCost> m_AryCostItem;
};

UENUM(BlueprintType)
enum class EEquipSlotType:uint8
{
	None,
	Weapon,
	Head,
	Torso,
	Hand,
	Leg,
	Feet,
	Ring,
	Neckless,
	Length UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FItemDataRow : public FCraftable//FCraftable 상속?
{
	GENERATED_USTRUCT_BODY()

public:
	FItemDataRow();

public:
	UPROPERTY(EditDefaultsOnly)
	EEquipSlotType m_ItemType;
	UPROPERTY(EditDefaultsOnly)
	int m_nPlayerEarnGoldSell = 50;
	UPROPERTY(EditDefaultsOnly)
    int m_nPlayerSpentGoldBuy = 50;
	UPROPERTY(EditDefaultsOnly)
	FColorDataHandle m_ColorHandle;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None && m_ItemType != EEquipSlotType::Weapon", EditConditionHides))
	TSoftObjectPtr<USkeletalMesh> m_ArmorMesh;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	FStatGroup m_EquipStats;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	FStatGroup m_EnchantStats;//this * level = enchant
	// UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	// TArray<TSubclassOf<UOptionBase>> m_Options;
	// UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::None", EditConditionHides))
	// TArray<TSubclassOf<UEE_Base>> m_EquipEffects;
	// UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ItemType != EEquipSlotType::Weapon", EditConditionHides))
	// TSubclassOf<UItemExecuteBase> m_ClassExeItem;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ClassExeItem != nullptr", EditConditionHides))
	FString m_InteractVariable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int m_nMaxStack = 1;
	UPROPERTY(EditDefaultsOnly)
	TArray<FDropData> m_AryDropDatas;
};