// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Active/SkillActiveBase.h"
#include "SkillActiveCircleDmg.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillActiveCircleDmg : public USkillActiveBase
{
	GENERATED_BODY()

public:
	USkillActiveCircleDmg();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float m_fRadius;
	UPROPERTY(EditDefaultsOnly)
	float m_fDamage;
};
