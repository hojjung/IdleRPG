// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CapeComponent.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UCapeComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Transient)
	UMaterialInstanceDynamic* m_MatInst;

	FName m_NameWindParam;
protected:
	virtual void BeginPlay() override;

public:
	void SetWindPower(float v);
};
