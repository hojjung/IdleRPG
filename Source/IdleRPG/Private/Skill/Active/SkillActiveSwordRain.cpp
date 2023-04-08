#include "Skill/Active/SkillActiveSwordRain.h"

USkillActiveSwordRain::USkillActiveSwordRain()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FoundHitEffect(TEXT("NiagaraSystem'/Game/04_VisualEffect/StylizedVFX-Atacks/Particles/NS_SwordsAttack.NS_SwordsAttack'"));
	
	m_Effect = FoundHitEffect.Object;

	m_fDelay = 1;

	m_fRadius = 1000;

	m_fStartDamage = 4.5f;
	
	m_fLevelUpDamage = 1.0f;
}

void USkillActiveSwordRain::OnSkillUse(AMyPlayerPawn* pawn)
{
	Super::OnSkillUse(pawn);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(pawn->GetWorld(), m_Effect, pawn->GetActorLocation(), pawn->GetActorRotation(),
		FVector(1),true, true, ENCPoolMethod::None, true);
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