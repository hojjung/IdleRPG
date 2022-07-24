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
	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;

	virtual void Update(float DeltaSeconds) override ;

	UPROPERTY(Transient)
	UMyAnimInstance* m_MyAnim = nullptr;
};
UCLASS()
class ENTITY_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	
public:
	UPROPERTY(Transient, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	FMyAnimInstanceProxy m_Proxy;
	UPROPERTY()
	AMyBasePawn* m_Owner;
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequenceBase* m_Idle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequenceBase* m_Run;
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadWrite)
	bool m_bIsMoving;
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &m_Proxy;
	}
	
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

private:
	bool IsSlotPlaying();
	
public:
	float PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate, FName StartSectionName);

	void StopAnimMontage();

	void UpdateFlag(float deltaTime);
};
