#include "Skill/Active/SkillActiveEffectCollison.h"



USkillActiveEffectCollison::USkillActiveEffectCollison()
{
	m_fRadius = 100;

	m_AryMonsters.Reserve(50);

	m_DmgType = EDmgType::DmgPhys;

	m_fStartDamage = 1;
	
	m_fLevelUpDamage = 0.2f;

	m_fDuration = 10;
}

void USkillActiveEffectCollison::BeginDestroy()
{
	Super::BeginDestroy();

	m_AryMonsters.Reset();

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
	}

	DestoryEffect();
}

FText USkillActiveEffectCollison::GetDescString(int lv)
{
	int RadiusInt = m_fRadius;

	int DmgInt = GetLevelPerDmg(lv) * 100.0f;
	
	FTextFormat T = NSLOCTEXT("USkillActiveEffectCollison","USkillActiveEffectCollison","{0} 범위 전체에 <Yellow>{1}%</> 데미지를 주는 검의 세례를 시전");

	FText Result = FText::Format(T, RadiusInt, DmgInt);

	return Result;
}

float USkillActiveEffectCollison::GetLevelPerDmg()
{
	return GetLevelPerDmg(m_SkillInst->m_nLevel);
}

float USkillActiveEffectCollison::GetLevelPerDmg(int lv)
{
	lv--;
	
	return m_fStartDamage + (lv * m_fLevelUpDamage);
}

void USkillActiveEffectCollison::UseSkill()
{
	Super::UseSkill();

	DestoryEffect();

	AMyPlayerPawn* PlPawn = UMyGameInstance::Get->GetPlayerPawn();

	UNiagaraComponent* Nia = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		PlPawn->GetWorld(), m_Effect, PlPawn->GetActorLocation(), FRotator::ZeroRotator, FVector(1), false);

	Nia->SetNiagaraVariableObject(TEXT("Owner"), this);
	
	m_Spawned = Nia;

	GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &USkillActiveEffectCollison::DestoryEffect, m_fDuration);
}

void USkillActiveEffectCollison::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem)
{
	for(const FBasicParticleData& D : Data)
	{
		OnTrace(D.Position);
	}
}

void USkillActiveEffectCollison::OnTrace(FVector pos)
{
	m_AryMonsters.Reset();
	
	AMyPlayerPawn* PlPawn = UMyGameInstance::Get->GetPlayerPawn();
	
	UMyLib::GetNearActors<AMonsterPawn>(pos, m_AryMonsters, m_fRadius);

	for(AMonsterPawn* Mob : m_AryMonsters)
	{
		Mob->MyTakeDamage(PlPawn, m_DmgType, GetLevelPerDmg());
	}
}

void USkillActiveEffectCollison::DestoryEffect()
{
	if(m_Spawned)
	{
		m_Spawned->DestroyInstance();
		m_Spawned = nullptr;
	}
}