// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillQuickEle.h"

#include "Blueprint/WidgetTree.h"
#include "Manager/MyGameInstance.h"
#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillInfo.h"

void UWidgetSkillQuickEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_bIsRegisterMode = false;

	m_ImgSkillIcon->SetVisibility(ESlateVisibility::Collapsed);

	m_ImgSkillAdd->SetVisibility(ESlateVisibility::Visible);
}

FReply UWidgetSkillQuickEle::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
	
	if(m_bIsRegisterMode)
	{
		m_OnClick.ExecuteIfBound(m_nIndex);
	}

	return FReply::Unhandled();
}

void UWidgetSkillQuickEle::SetIndex(int ndx)
{
	m_nIndex = ndx;
}

void UWidgetSkillQuickEle::SetRegisterMode()
{
	m_bIsRegisterMode = true;
	
}

void UWidgetSkillQuickEle::EquipSkill()
{
	m_ImgSkillAdd->SetVisibility(ESlateVisibility::Collapsed);

	m_ImgSkillIcon->SetVisibility(ESlateVisibility::Visible);
}

void UWidgetSkillQuickEle::UnequipSkill()
{
	m_ImgSkillAdd->SetVisibility(ESlateVisibility::Visible);

	m_ImgSkillIcon->SetVisibility(ESlateVisibility::Collapsed);
}
