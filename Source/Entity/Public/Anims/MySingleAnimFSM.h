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
	
	TWeakObjectPtr<UAnimMontage> m_Atk;
	
	TStrongObjectPtr<AMyBasePawn> m_Owner;

	bool m_bMoving;

	float m_fSlotTimer;

	float m_fSlotDur;

protected:
	UAnimSingleNodeInstance* GetInst();

	bool IsSlotPlaying(float delta);

public:
	void Update(float deltaTime);
	
	void PlaySlotAnim(UAnimSequenceBase* anim, float rate = 1.f);
	
	void Attack();
};
