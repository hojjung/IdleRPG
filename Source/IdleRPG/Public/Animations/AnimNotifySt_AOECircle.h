// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IdleRPG.h"
#include "MatineeCameraShake.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Player/CombatPawn.h"
#include "AnimNotifySt_AOECircle.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UAnimNotifySt_AOECircle : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifySt_AOECircle(const FObjectInitializer& obj);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EDmgType m_DmgType;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UMatineeCameraShake> m_ClassCamShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float m_RotYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AnimNotify")
	UMaterialInterface* m_MatDecalTemplate;
	UPROPERTY(Transient, BlueprintReadWrite, Category="AnimNotify")
	UMaterialInstanceDynamic* m_MatDynamic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FLinearColor m_DecalColorWant;
	
	TWeakObjectPtr<UDecalComponent> m_Decal;
	
	UPROPERTY(EditAnywhere)
	float m_EulerAngle = 60.f;
	UPROPERTY(EditAnywhere)
	float m_Radius = 256.f;
	UPROPERTY(EditAnywhere)
	float m_fDamage;
	
	float m_fTimer;
	
	float m_fDurationMax;

	FVector m_StartPos;

	FVector m_StartDir;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	
	bool TraceDamage(ACombatPawn* CPawn);

	bool TraceSphere(const ACombatPawn* instigator, TArray<ACombatPawn*>& outHits, float radius);
};
