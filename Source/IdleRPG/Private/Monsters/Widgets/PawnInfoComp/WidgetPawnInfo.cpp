// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfo.h"

void UWidgetPawnInfo::SetPawnInfo(const ACombatPawn* pawn)
{
	float HpPercent = pawn->GetHpPercent();

	m_HpBar->SetPercent(HpPercent);
}

void UWidgetPawnInfo::SetGreen()
{
	m_HpBar->WidgetStyle.SetFillImage(m_GreenBar);
}
