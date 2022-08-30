// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, hidecategories = (Object, Actor, Advanced, Navigation))
class ENTITY_API UUnitAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditAnywhere, Category = Bundle, meta = (AssetBundles = "Icon"))
	UTexture2D* m_Icon;
	UPROPERTY(EditAnywhere, Category = Bundle, meta = (AssetBundles = "Preview"))
	USkeletalMesh* m_BodyMesh;
	UPROPERTY(EditAnywhere, Category = Bundle)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditAnywhere, Category = Bundle)
	UAnimMontage* m_DeathMontage;
	UPROPERTY(EditAnywhere, Category = Bundle)
	UAnimMontage* m_TookHitMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* m_TakeHitEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* m_TakeHitSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Preview"))
	TSubclassOf<UAnimInstance> m_ClassAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_RotYawOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "34" ,UIMax = "88"))
	float m_fCapsuleRadius = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fShadowScale = 6.f;

	FORCEINLINE virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Unit"), GetFName());
	}

	FORCEINLINE virtual USkeletalMesh* GetSkMesh() const
	{
		return m_BodyMesh;
	}

	FORCEINLINE virtual UAnimMontage* GetBaseAtkAnim() const
	{
		return m_BaseAttackAnim;
	}
};
