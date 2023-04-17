// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillActiveEffectCollison.h"
#include "SkillActiveArrowRain.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillActiveArrowRain : public USkillActiveEffectCollison
{
	GENERATED_BODY()

public:
	USkillActiveArrowRain();

	virtual FText GetDescString(int lv) override;
};
