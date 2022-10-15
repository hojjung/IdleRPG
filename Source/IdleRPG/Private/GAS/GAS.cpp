// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GAS.h"

GAS::GAS(uint32 id)
{
	m_PtrID = id;

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
