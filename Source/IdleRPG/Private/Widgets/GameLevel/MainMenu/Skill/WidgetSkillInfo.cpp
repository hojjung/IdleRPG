// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillInfo.h"

void UWidgetSkillInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetSkillInfo::Close);
}

void UWidgetSkillInfo::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetSkillInfo::ShowInfo(const FSkillDataRow& row)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
