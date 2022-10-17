// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameModeBase.h"

/**
 * 기본 스테이지
 */
class IDLERPG_API MyGameModeDefaultStage : public  MyGameModeBase
{
public:
	MyGameModeDefaultStage();
	
	virtual ~MyGameModeDefaultStage() override;

	virtual void OnMonsterDead(AMonsterPawn* target) override;
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) override;
};
