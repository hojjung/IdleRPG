// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Active/SkillActiveArrowRain.h"

USkillActiveArrowRain::USkillActiveArrowRain()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FoundHitEffect(TEXT("NiagaraSystem'/Game/04_VisualEffect/RPGEffects/ParticlesNiagara/Archer/ArrowHail/NS_Archer_Arrow_Hail.NS_Archer_Arrow_Hail'"));
	
	m_Effect = FoundHitEffect.Object;

	m_fDelay = 1;

	m_fRadius = 1500;

	m_fStartDamage = 4.5f;
	
	m_fLevelUpDamage = 1.0f;
}

