// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include  "Math/BigInt.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BigIntLib.generated.h"

typedef TBigInt<512, true> BigInt;

UCLASS()
class IDLERPG_API UBigIntLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static const int UnitSymbolMax = 52;

	static const FText UnitSymbol[UnitSymbolMax];

	static FString GetAlphabetTextBigInt(const BigInt& vWant, int symbolCount = 0)
	{
		if (vWant == 0)
		{
			return FString::FromInt(0);
		}
		BigInt Mile = 10000;

		BigInt Cache = vWant;

		BigInt Remain;

		TArray<BigInt> NumList;

		int Count = 0;

		while (Cache >= 1) //Mile
		{
			Cache.DivideWithRemainder(Mile, Remain);

			NumList.Emplace(Remain.ToInt());

			Count++;
		}

		FString RetStr;

		int MaxIter = Count - 1;

		int SymbolCounter = 0;

		for (int i = MaxIter; i >= 0; i--)
		{
			if (NumList[i] != 0)
			{
				RetStr.Append(FString::FromInt(NumList[i].ToInt()));
			}

			if (i < UnitSymbolMax) //자릿수초과시 그냥 합처서
			{
				RetStr.Append(UnitSymbol[i].ToString());

				SymbolCounter++;

				if (symbolCount > 0 && SymbolCounter >= symbolCount)
				{
					break;
				}
			}
		}

		return RetStr;
	}

	static BigInt MultiplePercent(BigInt a, BigInt b, int aPrecisionCount = 1, int bPrecisionCount = 1)
	{
		a.Multiply(b);

		int PrecisionFactor = FMath::Pow(10, aPrecisionCount + bPrecisionCount);

		a.Divide(PrecisionFactor);

		return a;
	}
};
