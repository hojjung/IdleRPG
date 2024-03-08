// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Avatar/AvatarAddtionalVisuals.h"
#include "DataAsset/UnitAsset.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Preview/PreviewProxy.h"
#include "PreviewActor.generated.h"

UCLASS()
class ENTITY_API APreviewActor : public AActor, public IPreviewProxy
{
	GENERATED_BODY()

public:	
	APreviewActor();
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshBody;
	UPROPERTY()
	UAvatarAddtionalVisuals* m_Visual;
protected:
	FDelegateHandle Handle1;

	FDelegateHandle Handle2;

	bool m_bTouched;

	FRotator m_InitVisualRot;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void CalculateVisualActorRot(float delta);

public:
	UFUNCTION(BlueprintCallable)
	void SetEntity(const UUnitAsset* asset);

	void SetMeshScale(float s);
	
	virtual void SetIsTouched(bool b) override;

	virtual void RotatePawn(float delta_x) override;

public:
	UFUNCTION(BlueprintCallable)
	float PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate, FName StartSectionName, float sectionDur);
	UFUNCTION(BlueprintCallable)
	float GetSectionLength(FName sectionName, const UAnimMontage* anim_montage);
	UFUNCTION(BlueprintCallable)
	float PlayAnimMontageSetDuration(UAnimMontage* anim_montage, float setDur, FName StartSectionName);
	UFUNCTION(BlueprintCallable)
	void StopAnimMontage();
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetCurrentMontage();
	UFUNCTION(BlueprintCallable)
		bool PlayMontageIndexDur(UAnimMontage* anim_montage, int index, float dur);
};
