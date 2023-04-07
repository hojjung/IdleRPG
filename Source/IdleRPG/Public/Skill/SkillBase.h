// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "SkillBase.generated.h"


UENUM(BlueprintType)
enum class ESkillType :uint8
{
	Active,
	Buff,
	Passive
};

struct FSkillInven;
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
