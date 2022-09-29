// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MyCapeComponent.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UMyCapeComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UMyCapeComponent();
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Transient)
	UMaterialInstanceDynamic* m_MatInst;

	FName m_NameWindParam;

public:
	void Init(UStaticMesh* stm);
	
	void SetWindPower(float v);
};
