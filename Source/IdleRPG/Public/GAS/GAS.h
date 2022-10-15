// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BigInt/BigIntLib.h"
#include "Player/CombatPawn.h"

/**
 * 
 */
class IDLERPG_API GAS
{
public:
	GAS(uint32 id);
	
	~GAS();

private:
	uint32 m_PtrID;

	BigInt m_cHp;

	BigInt m_mHp;

	TMap<FName, FName> m_MapSkillInst;//temp

public:
	void Restart();

	float GetHpPercent() const;

	bool IsAlive() const;
	
	void TryExecuteSkill(const FName& id);
	
	void TakeDamage(ACombatPawn* combat_pawn, EDmgType dmg);
};
