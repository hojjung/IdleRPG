// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillBase : public UObject
{
	GENERATED_BODY()
	
};
UENUM(BlueprintType)
enum class ESkillType :uint8
{
	Active,
	Buff,
	Passive
};