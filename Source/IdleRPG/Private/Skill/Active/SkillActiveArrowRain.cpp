// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Active/SkillActiveArrowRain.h"

USkillActiveArrowRain::USkillActiveArrowRain()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FoundHitEffect(TEXT("NiagaraSystem'/Game/04_VisualEffect/RPGEffects/ParticlesNiagara/Archer/ArrowHail/NS_Archer_Arrow_Hail.NS_Archer_Arrow_Hail'"));
	
	m_Effect = FoundHitEffect.Object;

	m_fRadius = 200;

	m_fStartDamage = 0.7f;
	
	m_fLevelUpDamage = 0.15f;
}

FText USkillActiveArrowRain::GetDescString(int lv)
{
	int RadiusInt = m_fRadius;

	int Dur = m_fDuration;
	
	int DmgInt = GetLevelPerDmg(lv) * 100.0f;
	
	FTextFormat T = NSLOCTEXT("USkillActiveArrowRain","USkillActiveArrowRain","{0} 범위 전체에 {1}초 동안 <Yellow>{2}%</> 데미지를 주는 화살비를 내린다");

	FText Result = FText::Format(T, RadiusInt, Dur, DmgInt);

	return Result;
}

