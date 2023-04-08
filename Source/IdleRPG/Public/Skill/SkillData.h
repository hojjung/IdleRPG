// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/EntityData.h"
#include "Engine/DataTable.h"
#include "IdleRPG.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SkillData.generated.h"


class USkillBase;
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
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "m_ClassSkillBase->IsChildOf(USkillActiveBase::StaticClass())"))
	float m_fCooltime = 0;
};

USTRUCT(BlueprintType)//USkillActiveBase
struct FSkillInven
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int m_nCount = 0;
	UPROPERTY(EditAnywhere)
	int m_nLevel = 1;
	UPROPERTY(EditAnywhere)
	FName m_SkillID;
	UPROPERTY(EditAnywhere)
	float m_fCooldownRemain = 0;
	UPROPERTY(EditAnywhere)
	bool m_bUnlocked = false;
	UPROPERTY(EditAnywhere)
	bool m_bEquipped = false;
};


UENUM(BlueprintType)
enum class ESkillType :uint8
{
	Active,
	Buff,
	Passive
};

UCLASS()
class IDLERPG_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	USkillBase();
	
protected:
	UPROPERTY()
	FText m_TextDesc;

	FSkillInven* m_SkillInst;
	
public:
	virtual FText GetDescString(int lv);

	virtual void UseSkill();

	void SetSkillInst(FSkillInven* inst);
};
