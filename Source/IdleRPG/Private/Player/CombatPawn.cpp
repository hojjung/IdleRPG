#include "Player/CombatPawn.h"
#include "Kismet/KismetMathLibrary.h"

ACombatPawn::ACombatPawn(const FObjectInitializer& objInit) :Super(objInit)
{
	
}

void ACombatPawn::SetFocusedTarget(ACombatPawn* pawn)
{
	m_Target = pawn;
}

ACombatPawn* ACombatPawn::GetFocusedTarget()
{
	return m_Target.Get();
}

void ACombatPawn::HomingRotateToTarget(float speedTime)
{
	FRotator NewRot = GetActorRotation();

	if(speedTime <= 0)
	{
		NewRot.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetFocusedTarget()->GetActorLocation()).Yaw;
	}
	else
	{
		NewRot.Yaw = UKismetMathLibrary::RInterpTo(NewRot, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetFocusedTarget()->GetActorLocation()),GetWorld()->GetDeltaSeconds(), speedTime).Yaw;
	}

	SetActorRotation(NewRot);
}


float ACombatPawn::PlayBaseAttackAnim()
{
	const TArray<FCompositeSection>& AnimAry = m_EntityAsset->m_BaseAttackAnim->CompositeSections;
	
	int RandIndex = FMath::RandRange(0, AnimAry.Num()-1);
	
	return PlayAnimMontage(m_EntityAsset->m_BaseAttackAnim, 1, AnimAry[RandIndex].SectionName);
}

float ACombatPawn::TryAttack(float playRate)
{
	if (m_EntityAsset->m_BaseAttackAnim && m_fAttackCD < 0.f)
	{
		float AnimMongLen = PlayBaseAttackAnim();

		m_fAttackCD = FMath::Max(AnimMongLen - 0.1f,  playRate);

		return m_fAttackCD;
	}

	return 0.f;
}
