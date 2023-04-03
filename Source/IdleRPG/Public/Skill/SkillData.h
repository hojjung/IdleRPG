// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "DataTableRow/EntityData.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "SkillData.generated.h"



UCLASS()
class IDLERPG_API USkillData : public UObject
{
	GENERATED_BODY()
public:
	USkillData();

	static UDataTable* GetSkillData;
};
USTRUCT(BlueprintType)
struct FSkillDataRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()
	
public://buff actice passvie//스킬은 클래스에서 다 해주는게 낫지 않을까
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillBase> m_ClassSkillBase;
};

USTRUCT()
struct FSkillInven
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int m_nCount = 0;
	UPROPERTY(EditAnywhere)
	int m_nLevel = 0;
	UPROPERTY(EditAnywhere)
	FName m_SkillID;
	UPROPERTY(EditAnywhere)
	float m_fCooldown;
	UPROPERTY(EditAnywhere)
	bool m_bUnlocked;
	UPROPERTY(EditAnywhere)
	bool m_bEquipped;
};
