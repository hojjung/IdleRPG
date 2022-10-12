// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PetAsset.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UPetAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle ,meta = (AssetBundles = "Preview"))
	TSoftObjectPtr< USkeletalMesh> m_BodyMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<UAnimSequence> m_IdleAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_RotYawOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;

	FORCEINLINE virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Pet"), GetFName());
	}
};
