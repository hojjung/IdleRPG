// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BlendSpace1D.h"

#include "DataAsset/EntityBaseAsset.h"
#include "UObject/StrongObjectPtr.h"

class AMyBasePawn;
/**
 * 
 */
class ENTITY_API MySingleAnimFSM 
{
public:
	MySingleAnimFSM(const UUnitEntityAsset* asset, AMyBasePawn* pawn);
	
	~MySingleAnimFSM();

protected:
	TWeakObjectPtr<UBlendSpace1D> m_Loco;
	
	TStrongObjectPtr<AMyBasePawn> m_Owner;

	bool m_bMoving;

protected:
	UAnimSingleNodeInstance* GetInst();

public:
	void Update(float deltaTime);
	
};
