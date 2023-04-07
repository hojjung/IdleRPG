// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Active/SkillActiveCircleDmg.h"
#include "SkillActiveSwordRain.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillActiveSwordRain : public USkillActiveCircleDmg
{
	GENERATED_BODY()

public:
	USkillActiveSwordRain();

protected:
	UPROPERTY()
	UNiagaraSystem* m_Effect;
	
protected:
	virtual void OnSkillUse(AMyPlayerPawn* pawn) override;

	virtual void OnSkillDmged(AMonsterPawn* pawn) override;

	virtual FText GetDescString(int lv) override;
};
