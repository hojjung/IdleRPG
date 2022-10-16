// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BigInt/BigIntLib.h"

enum class EDmgType : uint8;
class ACombatPawn;
/**
 * 
 */
class IDLERPG_API GAS
{
public:
	GAS(uint32 id);
	
	~GAS();

	DECLARE_MULTICAST_DELEGATE(FOnHpChanged)
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTookDmg, BigInt)

	FOnHpChanged m_OnHpChanged;

	FOnTookDmg m_OnTookDamage;

private:
	uint32 m_PtrID;

	TMap<FName, FName> m_MapSkillInst;//temp
	
private:
	BigInt m_cHp;

	BigInt m_mHp;

	BigInt m_Dmg;

	BigInt m_DmgWeak;//속성별로있어야함
	
	BigInt m_CriPer;

	BigInt m_CriDmg;

	BigInt m_SuperCriPer;

	BigInt m_SuperCriDmg;


public:
	void Restart();

	float GetHpPercent() const;

	bool IsAlive() const;
	
	void TryExecuteSkill(const FName& id);

	void TakeDamage(ACombatPawn* combat_pawn, EDmgType dmg);
	
public:
	BigInt GetDmg(EDmgType dmg);
	
	BigInt GetCriDmg();
	
	BigInt GetDmgWeak(EDmgType dmg);
};
