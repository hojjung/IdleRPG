// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"


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
	UPROPERTY(EditDefaultsOnly)
	FText m_TextName;
	UPROPERTY(EditDefaultsOnly)
	FText m_TextDesc;

public:
	FText GetDescString();
};
