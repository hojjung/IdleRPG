// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfoComp.h"

#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfo.h"

UWidgetPawnInfoComp::UWidgetPawnInfoComp()
{
	SetManuallyRedraw(true);
}

void UWidgetPawnInfoComp::SetPawnInfo(const AMonsterPawn* pawn)
{
	UUserWidget* WidgetObj = GetUserWidgetObject();

	if (!WidgetObj)
	{
		return;
	}
	
	UWidgetPawnInfo* WidgetProgressBar = Cast<UWidgetPawnInfo>(WidgetObj);

	WidgetProgressBar->SetPawnInfo(pawn);

	RequestRedraw();
}
