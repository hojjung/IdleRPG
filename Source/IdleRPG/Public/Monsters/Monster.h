// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"

class AMonsterPawn;
/**
 *  쿼드트리?
 */
class IDLERPG_API Monster
{
public:
	Monster(AMonsterPawn* pawn, UUnitEntityAsset* asset);
	
	~Monster();

protected:
	TWeakObjectPtr<AMonsterPawn> m_Pawn;

	float m_fMaxHp;
	
	float m_fHp;

public:
	void Update(float delta);
	
	float GetHpPercent();
};
