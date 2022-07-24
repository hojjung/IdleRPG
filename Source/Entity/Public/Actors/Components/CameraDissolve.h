// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialParameterCollection.h"

#include "CameraDissolve.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENTITY_API UCameraDissolve : public USceneComponent
{
	GENERATED_BODY()

public:
	UCameraDissolve();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Lag, meta=(editcondition="bEnableCameraLag", ClampMin="0.0", ClampMax="1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float CameraLagSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag, AdvancedDisplay, meta=(editcondition = "bUseCameraLagSubstepping", ClampMin="0.005", ClampMax="0.5", UIMin = "0.005", UIMax = "0.5"))
	float CameraLagMaxTimeStep;
protected:
	FVector m_RelativeSocketLocation;

	FQuat m_RelativeSocketRotation;

	FVector m_PreviousArmOrigin;

	FVector m_PreviousDesiredLoc;

	FVector m_CamPos;

	bool m_bWasBlocked;

	float m_fDissolvingTime;

	float m_fTimer;

	float m_fDissloveAmount;

	float m_fDissolveHoleRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_fDissolveMaxAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_fDissolveHoleMaxRadius;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	FVector m_SocketOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float TargetArmLength;
protected:
	UPROPERTY(Transient,VisibleAnywhere)
	UMaterialParameterCollection* m_MatParamAsset;

public:
	void StartDissolve();

	void EndDissolve();

	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;

protected:
	void ExecuteDissolve(float DeltaTime);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SetValueParameter();
	//

	virtual void UpdateDesiredArmLocation(float DeltaTime);

	FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime);
};
