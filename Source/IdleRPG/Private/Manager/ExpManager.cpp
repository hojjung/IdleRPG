// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ExpManager.h"

ExpManager::ExpManager()
{
}

ExpManager::~ExpManager()
{
}

void ExpManager::AddExp(const BigInt& v, bool useBonus)
{
	m_CurrentExp += v;
}

float ExpManager::GetPercent()
{
	float PerOne = UBigIntLib::GetOnePercent(m_CurrentExp, m_MaxExp);

	return PerOne;
}

FText ExpManager::GetPercentText(float per)
{
	per *= 100.0f;

	FString PercentSter = FString::Printf(TEXT("%2.1f"), per);

	return FText::FromString(PercentSter);
}
