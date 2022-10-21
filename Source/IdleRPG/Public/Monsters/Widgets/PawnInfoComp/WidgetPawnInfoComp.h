// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WidgetPawnInfoComp.generated.h"

class UWidgetPawnInfo;
class ACombatPawn;
/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetPawnInfoComp  : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UWidgetPawnInfoComp();

protected:
	UPROPERTY()
	UWidgetPawnInfo* m_Info;
	
public:
	virtual void BeginPlay() override;
	
	void SetPawnInfo(const ACombatPawn* pawn);

	void SetGreen();
};