// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/EntityBaseAsset.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "EntityData.generated.h"

class AMyBasePawn;
/**
 * 
 */
UCLASS()
class ENTITY_API UEntityData : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FEntityRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine="true"))
	FText m_Desc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_Icon;

	virtual UStaticMesh* GetStMesh()
	{
		return nullptr;
	}
	virtual USkeletalMesh* GetSkMesh()
	{
		return nullptr;
	}
	virtual float GetMeshScale()
	{
		return 1.f;
	}
	
};

USTRUCT(BlueprintType)
struct FNpcUnitEntityRow : public FEntityRow
{
	GENERATED_BODY()

public:
	FNpcUnitEntityRow();
	
public://스텟과 보상
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UUnitEntityAsset> m_UnitDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMyBasePawn> m_ClassActor;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<UAI_LogicBase> m_ClassAI_Logic;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<USensor_LogicBase> m_ClassAI_Sensor;
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

	virtual USkeletalMesh* GetSkMesh() override;

	virtual float GetMeshScale() override;
};