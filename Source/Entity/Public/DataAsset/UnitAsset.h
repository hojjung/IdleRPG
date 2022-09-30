// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitAsset.generated.h"

UCLASS(Blueprintable, hidecategories = (Object, Actor, Advanced, Navigation))
class ENTITY_API UUnitAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Icon"))
	TSoftObjectPtr<UTexture2D> m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle ,meta = (AssetBundles = "Preview"))
	TSoftObjectPtr< USkeletalMesh> m_BodyMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<UAnimMontage> m_SpawnAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<UAnimMontage> m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<UAnimMontage> m_DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<UAnimMontage> m_TookHitMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<UParticleSystem> m_TakeHitEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle, meta = (AssetBundles = "Default"))
	TSoftObjectPtr<USoundBase> m_TakeHitSound;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle ,meta = (AssetBundles = "Preview"))
	TArray<TSoftObjectPtr<UStaticMesh>> m_AryAttachments;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Bundle)
	TArray<FName> m_ArySocketAttachments;

	FORCEINLINE virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Unit"), GetFName());
	}
};
