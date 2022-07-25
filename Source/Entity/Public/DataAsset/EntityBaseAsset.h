// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BlendSpace1D.h"
#include "Engine/DataAsset.h"
#include "EntityBaseAsset.generated.h"

UCLASS()
class ENTITY_API UEntityBaseAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FAttach
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* m_BackMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* m_LeftMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* m_RightMesh;
};

UCLASS(Blueprintable, hidecategories = (Object, Actor, Advanced, Navigation))
class ENTITY_API UUnitEntityAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public://Visual
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_RotYawOffset;
	UPROPERTY(EditAnywhere, Category = PrimaryAssetLabel, meta = (AssetBundles = "Icon"))
	UTexture2D* m_Icon;
	UPROPERTY(EditAnywhere, Category = PrimaryAssetLabel, meta = (AssetBundles = "Preview"))
	USkeletalMesh* m_BodyMesh;
	UPROPERTY(EditAnywhere, Category = PrimaryAssetLabel, meta = (AssetBundles = "Preview"))
	UBlendSpace1D* m_Loco;
	UPROPERTY(EditAnywhere, Category = PrimaryAssetLabel)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditAnywhere, Category = PrimaryAssetLabel)
	UAnimMontage* m_DeathMontage;
	UPROPERTY(EditAnywhere, Category = PrimaryAssetLabel)
	UAnimMontage* m_TookHitMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAttach m_Attach;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "34" ,UIMax = "88"))
	float m_fCapsuleRadius = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fShadowScale = 6.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* m_TakeHitEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* m_TakeHitSound;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Unit"), GetFName());
	}

	virtual USkeletalMesh* GetSkMesh() const;

	virtual UAnimMontage* GetBaseAtkAnim() const;
};
