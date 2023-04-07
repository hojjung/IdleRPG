// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Active/SkillActiveCircleDmg.h"

#include "MyLib.h"

USkillActiveCircleDmg::USkillActiveCircleDmg()
{
	m_fRadius = 100;

	m_fDelay = 0;

	m_AryMonsters.Reserve(50);

	m_DmgType = EDmgType::DmgPhys;
}

float USkillActiveCircleDmg::GetLevelPerDmg()
{
	return m_fDamage[m_SkillInst->m_nLevel - 1];
}

float USkillActiveCircleDmg::GetLevelPerDmg(int lv)
{
	return m_fDamage[lv - 1];
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
