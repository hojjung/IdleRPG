// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Active/SkillActiveSwordRain.h"

USkillActiveSwordRain::USkillActiveSwordRain()
{

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FoundHitEffect(TEXT("NiagaraSystem'/Game/04_VisualEffect/StylizedVFX-Atacks/Particles/NS_SwordsAttack.NS_SwordsAttack'"));
	
	m_Effect = FoundHitEffect.Object;

	m_fDelay = 1;

	m_fRadius = 700;

	m_fDamage[0] = 4.5f;
	m_fDamage[1] = 5.5f;
	m_fDamage[2] = 6.5f;
	m_fDamage[3] = 7.5f;
	m_fDamage[4] = 8.5f;
	m_fDamage[5] = 9.5f;
	m_fDamage[6] = 10.5f;
	m_fDamage[7] = 11.5f;
	m_fDamage[8] = 12.5f;
	m_fDamage[9] = 13.5f;
}

void USkillActiveSwordRain::OnSkillUse(AMyPlayerPawn* pawn)
{
	Super::OnSkillUse(pawn);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(pawn->GetWorld(), m_Effect, pawn->GetActorLocation(), pawn->GetActorRotation(),FVector(1),
		true, true, ENCPoolMethod::None, true);
}

void USkillActiveSwordRain::OnSkillDmged(AMonsterPawn* pawn)
{
	Super::OnSkillDmged(pawn);
	
}

FText USkillActiveSwordRain::GetDescString(int lv)
{
	int RadiusInt = m_fRadius;

	int DmgInt = GetLevelPerDmg(lv) * 100.0f;
	
	FTextFormat T = NSLOCTEXT("USkillActiveSwordRain","USkillActiveSwordRain","{0} 범위 전체에 <Yellow>{1}%</> 데미지를 주는 검의 세례를 시전");

	FText Result = FText::Format(T, RadiusInt, DmgInt);

	return Result;
}

