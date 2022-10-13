// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset/UnitAsset.h"
#include "PetAsset.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UPetAsset : public UUnitAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<UAnimSequence> m_IdleAnim;

	FORCEINLINE virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Pet"), GetFName());
	}
};
