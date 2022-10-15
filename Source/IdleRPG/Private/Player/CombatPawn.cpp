#include "Player/CombatPawn.h"
#include "Kismet/KismetMathLibrary.h"

ACombatPawn::ACombatPawn(const FObjectInitializer& objInit) :Super(objInit)
{
	SetAtkRange(130);

	m_ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
			TEXT("StaticMesh'/Game/03_VisualEffect/FX/Effects/FX_Meshes/SM_CharM_Shadow.SM_CharM_Shadow'"));
	m_ShadowMeshComp->SetStaticMesh(FoundSt.Object);
	m_ShadowMeshComp->SetupAttachment(GetSkMesh());
	m_ShadowMeshComp->SetRelativeLocation(FVector(0,0,5.f));
	m_ShadowMeshComp->SetRelativeScale3D(FVector(3));
	m_ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShadowMeshComp->SetCanEverAffectNavigation(false);
	m_ShadowMeshComp->bReceivesDecals = false;
}

void ACombatPawn::SetFocusedTarget(ACombatPawn* pawn)
{
	m_Target = pawn;
}

ACombatPawn* ACombatPawn::GetFocusedTarget()
{
	return m_Target.Get();
}

float ACombatPawn::PlayBaseAttackAnim()
{
	const TArray<FCompositeSection>& AnimAry = m_EntityAsset->m_BaseAttackAnim->CompositeSections;
	
	int RandIndex = FMath::RandRange(0, AnimAry.Num()-1);
	
	return PlayAnimMontage(m_EntityAsset->m_BaseAttackAnim.Get(), 1, AnimAry[RandIndex].SectionName);
}

void ACombatPawn::SetAtkRange(float v)
{
	m_fAtkRange = v;

	m_fAtkRangeSqr = m_fAtkRange * m_fAtkRange; 
}

void ACombatPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsAlive())
	{
		return;
	}
	OnTickAlive(DeltaSeconds);
}

void ACombatPawn::OnTickAlive(float DeltaSeconds)
{
	m_fAttackCD -= DeltaSeconds;
}

void ACombatPawn::SetEntity(const UUnitAsset* asset)
{
	Super::SetEntity(asset);

	m_ShadowMeshComp->SetRelativeScale3D(FVector(asset->m_fShadowScale));
}

float ACombatPawn::TryAttack(float playRate)
{
	if (IsAlive() && m_EntityAsset->m_BaseAttackAnim && m_fAttackCD < 0.f)
	{
		float AnimMongLen = PlayBaseAttackAnim();

		m_fAttackCD = FMath::Max(AnimMongLen - 0.1f,  0.15f);

		return m_fAttackCD;
	}

	return 0.f;
}

float ACombatPawn::GetAttackRange()
{
	return m_fAtkRange;
}

float ACombatPawn::GetAttackRangeSqr()
{
	return m_fAtkRangeSqr;
}

EPathFollowingRequestResult::Type ACombatPawn::ChaseTarget()
{
	return MoveToActor(GetFocusedTarget(), GetAttackRange());
}

float ACombatPawn::MyTakeDamage(float DamageAmount, EDmgType dmgType, ACombatPawn* DamageCauser)
{
	return 0;
}

void ACombatPawn::StartDie()
{
	m_OnDied.ExecuteIfBound(this);
}
