// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfo.h"

void UWidgetPawnInfo::SetPawnInfo(const AMonsterPawn* pawn)
{
	FText NamePawn =  pawn->GetPawnName();
	
	m_TextPawnName->SetText(NamePawn);

	float HpPercent = pawn->GetHpPercent();

	m_HpBar->SetPercent(HpPercent);
}
