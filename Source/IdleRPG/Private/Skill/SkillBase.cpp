// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillBase.h"

USkillBase::USkillBase()
{
	
}

FText USkillBase::GetDescString()
{
	return m_TextDesc;//FText::Format(row.m_Desc, Cd)
}

void USkillBase::UseSkill()
{
	
}
