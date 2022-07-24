// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "VDWindDirectionalSourceComponent.generated.h"

UCLASS(CollapseCategories, hidecategories = (Object, Mobility), editinlinenew)
class VICODYNAMICSPLUGIN_API UVDWindDirectionalSourceComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UVDWindDirectionalSourceComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintPure, Category = "VDynamics|Wind")
	float GetCurrentGust() const;

	/** if > 0, only affects VD Components that have a particle within this radius. NOTE: Can get expensive with many wind sources and VD Components.
	Alternatively, set this to 0 and add this Wind Source to the VD Component's 'Affected by Wind Sources' array */
	UFUNCTION(BlueprintCallable, Category = WindDirectionalSourceComponent)
	void SetRadius(float InRadius);

public:
	/** if > 0, only affects VD Components that have a particle within this radius. NOTE: Can get expensive with many wind sources and VD Components. 
	    Alternatively, set this to 0 and add this Wind Source to the VD Component's 'Affected by Wind Sources' array */
	UPROPERTY(Interp, EditAnywhere, BlueprintReadOnly, Category = WindDirectionalSourceComponent)
	float Radius;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float Strength;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float Speed;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MinGustAmount;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MaxGustAmount;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MinGustDuration;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MaxGustDuration;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MinGustTransitionSpeed;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MaxGustTransitionSpeed;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MinSecondsBetweenGusts;
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = WindDirectionalSourceComponent)
	float MaxSecondsBetweenGusts;

	float RadiusSquared;
	
protected:
	float CurrentGust;
	float TargetGust;
	float CurrentGustTransitionSpeed;
	float CurrentGustDurationLeft;
	FTimerHandle GustTimeoutTimerHandle;
};