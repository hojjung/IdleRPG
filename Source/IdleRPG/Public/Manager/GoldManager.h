// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BigInt/BigIntLib.h"

class IDLERPG_API GoldManager
{
public:
	DECLARE_MULTICAST_DELEGATE(FOnCurrencyChanged);

	FOnCurrencyChanged m_OnCurrencyChanged;

public:
	GoldManager();
	
	~GoldManager();

protected:
	BigInt m_CurrentGold;

public:
	BigInt AddGold(const BigInt& v,bool useBonus=true);

	bool SubtractGold(const BigInt& v);
	
	FORCEINLINE const BigInt& GetCurrentGold() const
	{
		return m_CurrentGold;
	}
};
