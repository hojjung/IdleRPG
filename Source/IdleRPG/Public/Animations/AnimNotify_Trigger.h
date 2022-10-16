// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Trigger.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UAnimNotify_Trigger : public UAnimNotify
{
	GENERATED_BODY()
protected:
   	UPROPERTY(EditAnywhere)
   	FName m_ID = TEXT("BaseAttack");
    
public:
   	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
