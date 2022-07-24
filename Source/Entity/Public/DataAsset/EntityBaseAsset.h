// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	USkeletalMesh* m_BodyMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_RotYawOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequenceBase* m_Idle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequenceBase* m_Run;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_TookHitMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAttach m_Attach; 

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Unit"), GetFName());
	}

	virtual USkeletalMesh* GetSkMesh() const;

	virtual UAnimMontage* GetBaseAtkAnim() const;
};
