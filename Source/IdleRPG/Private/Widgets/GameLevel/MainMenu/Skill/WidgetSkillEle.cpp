// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillEle.h"

#include "Skill/SkillData.h"

void UWidgetSkillEle::SetSkillDataOnClick(const FSkillInven& id, const FSkillDataRow& dataEquip, FOnClick onClick)
{
	SetDataOnClick(id.m_SkillID, dataEquip, onClick);

	
}
