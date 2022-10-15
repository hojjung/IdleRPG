// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BigIntLib.h"

/**
 * 
 */
class IDLERPG_API StatGroup
{
public:
	StatGroup();
	~StatGroup();

private:
	BigInt m_MaxHp;
	
	BigInt m_Hp;

	BigInt m_Dmg;
};
