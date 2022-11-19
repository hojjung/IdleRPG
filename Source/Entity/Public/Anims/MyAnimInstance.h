// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "MyAnimInstance.generated.h"

class UMyAnimInstance;
USTRUCT(BlueprintType)
struct FMyAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	FMyAnimInstanceProxy() : FAnimInstanceProxy()
	{
		m_bIsMoving = false;
	}

	FMyAnimInstanceProxy(UAnimInstance* Instance);
	
public:
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadOnly)
	bool m_bIsMoving;
	
public:
	virtual void Update(float DeltaSeconds) override ;
};
UCLASS()
class ENTITY_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	
public:
	UPROPERTY(Transient, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	FMyAnimInstanceProxy m_Proxy;
	
protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &m_Proxy;
	}
	
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
};