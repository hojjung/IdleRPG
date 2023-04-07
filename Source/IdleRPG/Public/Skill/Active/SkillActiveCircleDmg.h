// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/Active/SkillActiveBase.h"
#include "Player/CombatPawn.h"
#include "Player/MyPlayerPawn.h"
#include "Monsters/MonsterPawn.h"
#include "IdleRPG.h"
#include "SkillActiveCircleDmg.generated.h"


UCLASS()
class IDLERPG_API USkillActiveCircleDmg : public USkillActiveBase
{
	GENERATED_BODY()

public:
	USkillActiveCircleDmg();
	
protected:
	float m_fRadius;
	
	float m_fDamage[FGlobalVariable::SKILL_LEVEL_MAX];

	float m_fDelay;

	EDmgType m_DmgType;
	
	TArray<AMonsterPawn*> m_AryMonsters;
	
protected:
	float GetLevelPerDmg();

	float GetLevelPerDmg(int lv);
	
	virtual void UseSkill() override;

	void DelaySkill();

	virtual void BeginDestroy() override;

	virtual void OnSkillUse(AMyPlayerPawn* pawn);

	virtual void OnSkillDmged(AMonsterPawn* pawn);
};
