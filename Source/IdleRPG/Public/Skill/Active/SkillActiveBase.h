// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillActiveBase.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillActiveBase : public USkillBase
{
	GENERATED_BODY()

public:
	USkillActiveBase();

protected:
	UPROPERTY(EditDefaultsOnly)
	float m_fMaxCooltime;
};
