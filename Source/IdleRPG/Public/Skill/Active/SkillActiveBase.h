// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillData.h"
#include "Monsters/MonsterPawn.h"
#include "Player/MyPlayerPawn.h"
#include "Manager/MyGameInstance.h"
#include "MyLib.h"
#include "SkillActiveBase.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillActiveBase : public USkillBase
{
	GENERATED_BODY()

public:
	USkillActiveBase();
};
