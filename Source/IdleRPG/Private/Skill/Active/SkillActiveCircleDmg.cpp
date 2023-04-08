// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Active/SkillActiveCircleDmg.h"

#include "MyLib.h"

USkillActiveCircleDmg::USkillActiveCircleDmg()
{
	m_fRadius = 100;

	m_fDelay = 0;

	m_AryMonsters.Reserve(50);

	m_DmgType = EDmgType::DmgPhys;

	m_fStartDamage = 1;
	
	m_fLevelUpDamage = 0.2f;
}

float USkillActiveCircleDmg::GetLevelPerDmg()
{
	return GetLevelPerDmg(m_SkillInst->m_nLevel);
}

float USkillActiveCircleDmg::GetLevelPerDmg(int lv)
{
	lv--;
	
	return m_fStartDamage + (lv * m_fLevelUpDamage);
}

void USkillActiveCircleDmg::UseSkill()
{
	Super::UseSkill();

	AMyPlayerPawn* PlPawn = UMyGameInstance::Get->GetPlayerPawn();

	OnSkillUse(PlPawn);

	FTimerHandle Handle;
	
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &USkillActiveCircleDmg::DelaySkill, m_fDelay);
}

void USkillActiveCircleDmg::DelaySkill()
{
	AMyPlayerPawn* PlPawn = UMyGameInstance::Get->GetPlayerPawn();

	m_AryMonsters.Reset();
	
	UMyLib::GetNearActors<AMonsterPawn>(PlPawn, m_AryMonsters, m_fRadius, false);

	for(AMonsterPawn* Mob : m_AryMonsters)
	{
		Mob->MyTakeDamage(PlPawn, m_DmgType, GetLevelPerDmg());
		OnSkillDmged(Mob);
	}
}

void USkillActiveCircleDmg::BeginDestroy()
{
	Super::BeginDestroy();

	m_AryMonsters.Reset();
}

void USkillActiveCircleDmg::OnSkillUse(AMyPlayerPawn* pawn)
{
	
}

void USkillActiveCircleDmg::OnSkillDmged(AMonsterPawn* pawn)
{
	
}
