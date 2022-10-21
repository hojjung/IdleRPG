// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfoComp.h"

#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfo.h"

UWidgetPawnInfoComp::UWidgetPawnInfoComp()
{
	SetManuallyRedraw(true);
}
void UWidgetPawnInfoComp::BeginPlay()
{
	Super::BeginPlay();

	m_Info = Cast<UWidgetPawnInfo>(GetUserWidgetObject());
}

void UWidgetPawnInfoComp::SetPawnInfo(const ACombatPawn* pawn)
{
	m_Info->SetPawnInfo(pawn);

	RequestRedraw();
}

void UWidgetPawnInfoComp::SetGreen()
{
	m_Info->SetGreen();
}
