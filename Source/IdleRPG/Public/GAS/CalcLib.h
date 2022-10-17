// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BigInt/BigIntLib.h"
#include "Blueprint/UserWidget.h"
#include "CalcLib.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UCalcLib : public UUserWidget
{
	GENERATED_BODY()

public:
	static BigInt GetGoldPerLevel(int stage)
	{
		BigInt Result;

		return Result;
	}
};
