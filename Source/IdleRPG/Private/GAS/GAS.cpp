// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GAS.h"

#include "Player/CombatPawn.h"

GAS::GAS(uint32 id)
{
	m_PtrID = id;

	m_mHp = 100;
	
	m_Dmg = 5;

	Restart();
}

GAS::~GAS()
{
	
}

void GAS::Restart()
{
	m_cHp = m_mHp;
}

float GAS::GetHpPercent() const
{
	BigInt CopiedCH = m_cHp;

	CopiedCH.Multiply(100);

	CopiedCH.Divide(m_mHp);
    
	float Percent100 = CopiedCH.ToInt();

	float Percent1 =  Percent100 / 100.f;

	Percent1 = FMath::Clamp(Percent1,0.f,1.f);
    
	return Percent1;
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

void GAS::TakeDamage(ACombatPawn* combat_pawn, EDmgType dmg)
{
	GAS* Instigator = combat_pawn->GetGas();
	
	BigInt Damage = Instigator->GetDmg(dmg);

	BigInt FinalDmg = Damage;

	BigInt CriBonus = Instigator->GetCriDmg();

	FinalDmg = UBigIntLib::MultiplePercent(FinalDmg, CriBonus);
	
	BigInt DamageReduction = GetDmgWeak(dmg);

	FinalDmg = UBigIntLib::MultiplePercent(FinalDmg, DamageReduction);

	m_cHp.Subtract(FinalDmg);

	m_OnTookDamage.Broadcast(FinalDmg);
	
	m_OnHpChanged.Broadcast();
}

BigInt GAS::GetDmg(EDmgType dmg)
{
	return m_Dmg;
}

BigInt GAS::GetCriDmg()
{
	return 100;
}

BigInt GAS::GetDmgWeak(EDmgType dmg)
{
	return 100;//bonus damage so more than 1 is correct, less 1 is reduct
}
