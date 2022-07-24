// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Components/MeshComponent.h"
#include "Components/SplineComponent.h"

#include "VDRopeComponent.h"
#include "VDSimulation.h"
#include "VDSimulatedParticle.h"

#include "VDSplineRopeComponent.generated.h"

/**
 * Uses a SplineComponent for conforming the shape of the rope
 */
UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class VICODYNAMICSPLUGIN_API UVDSplineRopeComponent : public UVDRopeComponent
{
public:
	GENERATED_BODY()

	UVDSplineRopeComponent(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Begin UObject interface.
 #if WITH_EDITOR
 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
 #endif // WITH_EDITOR
	// End UObject interface.

	// Begin UBaseVDComponent interface.
	virtual void RegisterWithSimulation() override;
	// End UBaseVDComponent interface.

	// Begin UVDRopeComponent interface.
	virtual void CreateRopeGeometry(TArray<FVDParticleInfo>& RopePoints) override;
	// End UVDRopeComponent interface.

protected:
	/* The Spline to follow */
	UPROPERTY(EditAnywhere, Category = "Spline")
	FComponentReference SplineRef;

	/* If true, will auto create attachments to the end points of the Spline. NOTE: AttachToRigids must be off/false */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta = (EditCondition = "!AttachToRigids"))
	uint32 bAutoAttachToSplineEndPoints : 1;

	/* If true, will stretch the rope to the length of the Spline while keeping the rest length set to the Ropes Length property. Helps add tension. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	uint32 bStretchToSplineLength : 1;

private:
	/* The current length of the referenced Spline, for setting the rope length */
	UPROPERTY(VisibleAnywhere, Category = "Spline")
	float SplineLength;
};
