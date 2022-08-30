// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyStat.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UMyStat : public UObject
{
	GENERATED_BODY()
	
};


USTRUCT(BlueprintType)
struct FStatGroup 
{
	GENERATED_BODY()

public:
	FStatGroup()
	{
		m_nAccu = 10;
		m_nAvoid = 0;
		m_MaxHp = 100;
		m_Hp = 100;
		m_Dmg = 10;
		m_AtkPerSec = 1.0f;
		m_MoveSpeed = 300;
		m_CriPer = 0;
		m_DmgReduce = 0;
		m_CriDmg = 1.5f;

		m_nDef = 0;
		m_Penet= 0;
	}

	FStatGroup(float initAll)//for multiplier
	{
		m_nAvoid    = initAll;
		m_nAccu     = initAll;
		m_MaxHp     = initAll;
		m_Hp        = initAll;
		m_Dmg       = initAll;
		m_AtkPerSec  = initAll;
		m_MoveSpeed = initAll;
		m_CriPer    = initAll;
		m_CriDmg    = initAll;
		m_DmgReduce = initAll;

		m_nDef = initAll;
		m_Penet= initAll;
	}

public:
	UPROPERTY(EditDefaultsOnly)
	int m_nAccu;
	UPROPERTY(EditDefaultsOnly)
	int m_nAvoid;
	UPROPERTY(EditDefaultsOnly)
	int m_nDef;
	UPROPERTY(EditDefaultsOnly)
	float m_MaxHp;
	UPROPERTY(BlueprintReadOnly)
	float m_Hp;
	UPROPERTY(EditDefaultsOnly)
	int m_Dmg;
	UPROPERTY(EditDefaultsOnly)
	int m_Penet;
	UPROPERTY(EditDefaultsOnly)
	int m_DmgReduce;
	UPROPERTY(EditDefaultsOnly)
	float m_CriPer;
	UPROPERTY(EditDefaultsOnly)
	float m_CriDmg;
	UPROPERTY(EditDefaultsOnly)
	float m_AtkPerSec;
	UPROPERTY(EditDefaultsOnly)
	float m_MoveSpeed;
	
	FStatGroup  operator + (const FStatGroup& stat_group) const;
	
	FStatGroup  operator * (const FStatGroup& stat_group) const;

	FStatGroup  operator - (const FStatGroup& stat_group) const;
	
	FStatGroup  operator / (const FStatGroup& stat_group) const;

	FStatGroup  operator * (float m) const;

	FStatGroup  operator * (int m) const;

	FStatGroup& operator+= (const FStatGroup& stat_group);

	FStatGroup& operator-=(const FStatGroup& stat_group);
};

