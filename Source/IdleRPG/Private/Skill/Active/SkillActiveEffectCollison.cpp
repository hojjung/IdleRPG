#include "Skill/Active/SkillActiveEffectCollison.h"
#include "Monsters/MonsterPawn.h"

USkillActiveEffectCollison::USkillActiveEffectCollison()
{
	m_fRadius = 100;

	m_fDelay = 0;

	m_AryMonsters.Reserve(50);

	m_DmgType = EDmgType::DmgPhys;

	m_fStartDamage = 1;
	
	m_fLevelUpDamage = 0.2f;
}

void USkillActiveEffectCollison::BeginDestroy()
{
	Super::BeginDestroy();

	m_AryMonsters.Reset();
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

	AMyPlayerPawn* PlPawn = UMyGameInstance::Get->GetPlayerPawn();
	
	auto* Nia = UNiagaraFunctionLibrary::SpawnSystemAtLocation(PlPawn->GetWorld(), m_Effect, PlPawn->GetActorLocation(), PlPawn->GetActorRotation(),
		FVector(1),true, true, ENCPoolMethod::None, true);

	Nia->SetNiagaraVariableObject(TEXT("Owner"), PlPawn);
}

void USkillActiveEffectCollison::ReceiveParticleData(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem)
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