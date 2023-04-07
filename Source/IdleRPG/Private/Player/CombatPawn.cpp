#include "Player/CombatPawn.h"

#include "Actors/Components/MyNavMovement.h"

ACombatPawn::ACombatPawn(const FObjectInitializer& objInit) :Super(objInit)
{
	SetAtkRange(130);

	m_ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
			TEXT("StaticMesh'/Game/03_VisualEffect/FX/Effects/FX_Meshes/SM_CharM_Shadow.SM_CharM_Shadow'"));
	m_ShadowMeshComp->SetStaticMesh(FoundSt.Object);
	m_ShadowMeshComp->SetupAttachment(GetRootComponent());
	m_ShadowMeshComp->SetRelativeLocation(FVector(0, 0, -85));
	m_ShadowMeshComp->SetRelativeScale3D(FVector(3));
	m_ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_ShadowMeshComp->SetCanEverAffectNavigation(false);
	m_ShadowMeshComp->bReceivesDecals = false;

	m_fAtkDur = 1;
}

void ACombatPawn::SetFocusedTarget(ACombatPawn* pawn)
{
	m_Target = pawn;
}

ACombatPawn* ACombatPawn::GetFocusedTarget()
{
	return m_Target.Get();
}

float ACombatPawn::PlayBaseAttackAnim(float dur)
{
	const TArray<FCompositeSection>& AnimAry = m_EntityAsset->m_BaseAttackAnim->CompositeSections;
	
	int RandIndex = FMath::RandRange(0, AnimAry.Num()-1);
	
	return PlayAnimMontageSetDuration(m_EntityAsset->m_BaseAttackAnim.Get(), dur, AnimAry[RandIndex].SectionName);
}

void ACombatPawn::SetAtkRange(float v)
{
	m_fAtkRange = v;

	m_fAtkRangeSqr = m_fAtkRange * m_fAtkRange; 
}

void ACombatPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_fAttackCD -= DeltaSeconds;
}

void ACombatPawn::OnNotifyTrigger(const FName& id)
{
	if(id == TEXT("BaseAttack"))
	{
		if(!GetFocusedTarget())
		{
			return;
		}

		ACombatPawn* Pawn =  GetFocusedTarget();

		if(!Pawn)
		{
			return;
		}

		Pawn->MyTakeDamage(this,EDmgType::DmgPhys);
		return;
	}
	m_Gas.Pin()->TryExecuteSkill(id);
}

void ACombatPawn::SetGas(TSharedPtr<GAS> newGas)
{
	m_Gas = newGas;

	m_Gas.Pin()->m_OnDead.AddUObject(this, &ACombatPawn::OnDead);
}

void ACombatPawn::OnDead()
{
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetFocusedTarget(nullptr);

	m_Movement->SetComponentTickEnabled(false);

	PlayDeathAnim();
}

void ACombatPawn::PlayDeathAnim()
{
	if(m_EntityAsset->m_DeathMontage)
	{
		float AnimLength = PlayAnimMontageSetDuration(m_EntityAsset->m_DeathMontage.Get()) - 0.4f;

		GetWorldTimerManager().SetTimer(m_DeathAnimTimer, this, &ACombatPawn::OnDeathAnimEnd, AnimLength, false);
	}
	else
	{
		OnDeathAnimEnd();
	}
}

void ACombatPawn::OnDeathAnimEnd()
{
	m_BodyMesh->bPauseAnims = true;
	
	SetActorTickEnabled(false);
			
	SetActorHiddenInGame(true);
}


void ACombatPawn::Revive()
{
	SetActorLocation(m_SpawnPos);
	
	SetActorRotation(FRotator(0,FMath::RandRange(0,360),0));
	
	SetActorHiddenInGame(false);
	
	m_BodyMesh->bPauseAnims = false;
	
	m_Capsule->SetCollisionEnabled(m_InitColl);
	//
	if(m_EntityAsset->m_SpawnAnim.Get())
	{
		PlayAnimMontage(m_EntityAsset->m_SpawnAnim.Get());
		
		float AnimLength = m_EntityAsset->m_SpawnAnim->GetPlayLength() - 0.4f;
		//
		GetWorldTimerManager().SetTimer(m_DeathAnimTimer, this, &ACombatPawn::OnReviveAnimEnd, AnimLength, false);
		return;
	}
	OnReviveAnimEnd();
}

void ACombatPawn::OnReviveAnimEnd()
{
	m_ShadowMeshComp->SetVisibility(true);

	m_Movement->SetComponentTickEnabled(true);

	SetActorTickEnabled(true);
	
	m_Gas.Pin()->Restart();
}


GAS* ACombatPawn::GetGas()
{
	return m_Gas.Pin().Get();
}

void ACombatPawn::SetEntity(const UUnitAsset* asset)
{
	Super::SetEntity(asset);

	m_InitColl = m_Capsule->GetCollisionEnabled();

	m_ShadowMeshComp->SetRelativeScale3D(FVector(FMath::Max(1.0f, asset->m_fScale)  * 6.0f));
}

float ACombatPawn::TryAttack()
{
	if (IsAlive() && m_EntityAsset->m_BaseAttackAnim && m_fAttackCD < 0.f)
	{
		float AnimMongLen = PlayBaseAttackAnim(m_fAtkDur);

		m_fAttackCD = FMath::Max(AnimMongLen ,  0.15f);

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
	HomingRotateToTarget(15, GetFocusedTarget()->GetActorLocation());
	return MoveToActor(GetFocusedTarget(), GetAttackRange() - 12.0f);
}

void ACombatPawn::MyTakeDamage(ACombatPawn* DamageCauser, EDmgType dmgType, float dmgMultiple)
{
	m_Gas.Pin()->TakeDamage(DamageCauser, dmgType, dmgMultiple);
}

bool ACombatPawn::IsAlive()
{
	return m_Gas.Pin()->IsAlive();
}

float ACombatPawn::GetHpPercent() const
{
	return m_Gas.Pin()->GetHpPercent();
}

void ACombatPawn::AddComp(UObject* key, UActorComponent* get)
{
	m_MapComp.Emplace(key, TStrongObjectPtr<UActorComponent>(get));
}

UActorComponent* ACombatPawn::FindComp(UObject* key)
{
	TStrongObjectPtr<UObject> KeyWrap (key);
	
	TStrongObjectPtr<UActorComponent>* Found = m_MapComp.Find(KeyWrap);

	if(!Found)
	{
		return nullptr;
	}
	return Found->Get();
}
