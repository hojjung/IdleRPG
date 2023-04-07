// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillBase.h"

USkillBase::USkillBase()
{
	m_SkillInst = nullptr;
}

FText USkillBase::GetDescString(int lv)
{
	return m_TextDesc;//FText::Format(row.m_Desc, Cd)
}

void USkillBase::UseSkill()
{
	
}

void USkillBase::SetSkillInst(FSkillInven* inst)
{
	m_SkillInst = inst;
}
