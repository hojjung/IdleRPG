// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Monsters/MonsterPawn.h"
#include "WidgetPawnInfo.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetPawnInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_HpBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush m_GreenBar;
public:
	void SetPawnInfo(const ACombatPawn* pawn);

	void SetGreen();
};
