// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BigIntLib.h"
#include "DataTableRow/EntityData.h"
#include "UObject/NoExportTypes.h"
#include "BigIntCalcTableBase.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UBigIntCalcTableBase : public UObject
{
	GENERATED_BODY()
public:
	UBigIntCalcTableBase();

	static UDataTable* GetBigIntTable;
};
USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FBigIntCalcTableRow : public FEntityDataRow
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int m_nMaxLevel = 1000;
	UPROPERTY(EditDefaultsOnly)
	float m_fInitCost = 1;
	UPROPERTY(EditDefaultsOnly)
	float m_fCoefficient = 1;
	UPROPERTY(EditDefaultsOnly)
	float m_fInitValue = 1;
	UPROPERTY(EditDefaultsOnly)
	float m_fIncValue = 1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,meta = (MultiLine="true"))
	FString m_DescFormat = "Current:{0}>>{1}";
	
public:
	BigInt GetCost(int level) const
	{
		return UBigIntLib::GetExponentValue(level, m_fInitCost, m_fCoefficient);
	}

	BigInt GetValue(int level) const
	{
		return UBigIntLib::GetLinearValue(level, m_fInitValue, m_fIncValue);
	}

	FText GetDesc(int level) const
	{
		if(level >= m_nMaxLevel)
		{
			return FText::FromString("MAX");
		}

		FTextFormat Format = FText::FromString(m_DescFormat);

		FFormatOrderedArguments Args;
		
		FString Current = UBigIntLib::GetAlphabetTextBigInt(GetValue(level));
		
		Args.Add(FText::FromString(Current));
		
		FString Next;
		
		int NewLevel = level + 1;

		if(NewLevel >= m_nMaxLevel)
		{
			Next = TEXT("MAX");
		}
		else
		{
			Next = UBigIntLib::GetAlphabetTextBigInt(GetValue(NewLevel));
		}

		Args.Add(FText::FromString(Next));
		
		FText TT = FText::Format(Format, Args);

		return TT; 
	}
};