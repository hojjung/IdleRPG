// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GAS.h"

#include "Player/CombatPawn.h"

GAS::GAS(uint32 id)
{
	m_PtrID = id;

	m_mHp = 100;
	
	m_Dmg = 25;

	Restart();
}

GAS::~GAS()
{
	
}

void GAS::UpdateHpPercent(float v)
{
	m_cHp = m_mHp * v;
}

void GAS::SetDefaultStat(BigInt mHp, BigInt dmg)
{
	float V = GetHpPercent();
	
	m_mHp = mHp;
	
	UpdateHpPercent(V);
	
	m_Dmg = dmg;
}

void GAS::Restart()
{
	m_cHp = m_mHp;
}

float GAS::GetHpPercent() const
{
	float OnePer = UBigIntLib::GetOnePercent(m_cHp, m_mHp);
	
	return OnePer;
}

bool GAS::IsAlive() const
{
	return !m_cHp.IsLessThanZero();
}

void GAS::TryExecuteSkill(const FName& id)
{
	FName* FoundValue = m_MapSkillInst.Find(id);

	if(!FoundValue)//not contain = nullptr ㅇㅇ
	{
		return;
	}
	//AsD.
}

void GAS::TakeDamage(ACombatPawn* combat_pawn, EDmgType dmg, float dmgMultiple)
{
	if(!IsAlive())
	{
		return;
	}
	EDamagePopup Pop = EDamagePopup::Normal;
	
	GAS* Instigator = combat_pawn->GetGas();
	
	BigInt Damage = Instigator->GetDmg(dmg);

	Damage = UBigIntLib::MultiplePercent(Damage, dmgMultiple * 100.0f);

	BigInt FinalDmg = Damage;

	BigInt CriBonus = Instigator->GetCriDmg(Pop);

	FinalDmg = UBigIntLib::MultiplePercent(FinalDmg, CriBonus);
	
	BigInt DamageReduction = GetDmgWeak(dmg);

	FinalDmg = UBigIntLib::MultiplePercent(FinalDmg, DamageReduction);

	int RandRange = FMath::RandRange(80,120);
	
	FinalDmg = UBigIntLib::MultiplePercent(FinalDmg, RandRange);

	m_cHp.Subtract(FinalDmg);

	m_OnTookDamage.Broadcast(combat_pawn, FinalDmg, Pop);
	
	m_OnHpChanged.Broadcast();

	if(!IsAlive())
	{
		combat_pawn->SetFocusedTarget(nullptr);
		m_OnDead.Broadcast();
	}
}

BigInt GAS::GetDmg(EDmgType dmg)
{
	return m_Dmg;
}

BigInt GAS::GetCriDmg(EDamagePopup& outPop)
{
	outPop = EDamagePopup::Normal;
	return 100;
}

BigInt GAS::GetDmgWeak(EDmgType dmg)
{
	return 100;//bonus damage so more than 1 is correct, less 1 is reduct
}
