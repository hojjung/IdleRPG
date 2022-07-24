// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Components/PoseableMeshComponent.h"

#include "VDRopeComponent.h"
#include "VDSimulation.h"
#include "VDSimulatedParticle.h"

#include "VDSkinnedRopeComponent.generated.h"

/**
 * Uses a Poseable mesh and its bone chain to create the rope and use the poseable mesh as the visual representation.
 */
UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class VICODYNAMICSPLUGIN_API UVDSkinnedRopeComponent : public UVDRopeComponent
{
public:
	GENERATED_BODY()

	UVDSkinnedRopeComponent(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Begin UPrimitiveComponent interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	// End UPrimitiveComponent interface.

	// Begin UBaseVDComponent interface.
	virtual void RegisterWithSimulation() override;
	virtual void Sync() override;
	// End UBaseVDComponent interface.

	// Begin UVDRopeComponent interface.
	virtual void CreateRopeGeometry(TArray<FVDParticleInfo>& RopePoints) override;
	virtual void BuildSimulatedRope() override;
	// End UVDRopeComponent interface.

protected:
	/* The Poseable mesh */
	UPROPERTY(EditAnywhere, Category = "Spline")
	FComponentReference PoseableMeshRef;

	TArray<FName> Bones;
};
