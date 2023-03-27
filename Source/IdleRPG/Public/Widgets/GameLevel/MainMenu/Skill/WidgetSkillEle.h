// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"
#include "Skill/SkillData.h"
#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipMenuEle.h"
#include "WidgetSkillEle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetSkillEle : public UWidgetEquipMenuEle
{
	GENERATED_BODY()


public:
	void SetSkillDataOnClick(const FSkillInven& id, const FSkillDataRow& dataEquip, FOnClick onClick);
};
