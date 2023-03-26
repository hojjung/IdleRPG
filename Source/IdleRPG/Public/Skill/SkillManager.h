// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillEquip.h"
#include "SkillInventory.h"

/**
 * 
 */
class IDLERPG_API SkillManager
{
public:
	SkillManager();
	
	~SkillManager();

public:
	void SetSkill(const TArray<FSkillInven>& ary);
	
public:
	TSharedPtr<SkillInventory> m_SkillInven;

	TSharedPtr<SkillEquip> m_SkillEquip;
};
