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

public://Visual//일단 기본적으로 동적 로드는 다돌아가준다, 근데 아이콘 같은것을 따로 로드해야하는 상황이있다.
	//아이콘을 평소에는 안쓴다. 그럼 로드할 필요가 없어서 Soft포인터로 준다음
	//번들 네임으로 명시했을때 추가 적인 로드를 해주는 개념이다.
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_ClassAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAttach m_Attach;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_RotYawOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "34" ,UIMax = "88"))
	float m_fCapsuleRadius = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fShadowScale = 6.f;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Unit"), GetFName());
	}

	virtual USkeletalMesh* GetSkMesh() const;

	virtual UAnimMontage* GetBaseAtkAnim() const;
};
