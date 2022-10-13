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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USceneCaptureComponent2D* m_Capture;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USpringArmComponent* m_Spring;
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
	void SetEntity(const UUnitAsset* asset);

	void SetMeshScale(float s);
	
	void ShowMeshWithTick();

	void HideMeshWithTick();

	virtual void SetIsTouched(bool b) override;

	virtual void RotatePawn(float delta_x) override;
	
	void SetCamSize(float cam_size);
	
	void SetZOffset(float z_offset);
};
