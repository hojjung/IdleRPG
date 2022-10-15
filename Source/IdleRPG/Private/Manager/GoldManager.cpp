// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GoldManager.h"

GoldManager::GoldManager()
{
	//m_nOfflineMinutes = -1;
	//m_bIsReceivedOfflineGoldThisTime = false;
}

GoldManager::~GoldManager()
{
}

BigInt GoldManager::AddGold(const BigInt& v, bool useBonus)
{
	BigInt FinalV = v;
	
	m_CurrentGold.Add(FinalV);

	m_OnCurrencyChanged.Broadcast();

	return FinalV;
}

bool GoldManager::SubtractGold(const BigInt& v)
{
	if (m_CurrentGold < v)
	{
		return false;
	}

	m_CurrentGold.Subtract(v);

	m_OnCurrencyChanged.Broadcast();

	return true;
}
