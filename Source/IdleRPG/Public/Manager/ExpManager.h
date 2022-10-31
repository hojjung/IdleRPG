// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BigInt/BigIntLib.h"

/**
 * 
 */
class IDLERPG_API ExpManager
{
public:
	DECLARE_MULTICAST_DELEGATE(FOnCurrencyChanged);

	FOnCurrencyChanged m_OnExpChanged;

public:
	ExpManager();
	
	~ExpManager();

protected:
	BigInt m_CurrentExp;

	BigInt m_MaxExp;

public:
	void AddExp(const BigInt& v, bool useBonus=true);
	
	FORCEINLINE const BigInt& GetCurrentExp() const
	{
		return m_CurrentExp;
	}

	float GetPercent();

	FText GetPercentText(float per);
};
