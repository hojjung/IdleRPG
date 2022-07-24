// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "VDSimulation.h"

#include "VDColliderComponent.generated.h"

/**
 * Component for explicitly marking Actor Components as Colliders when using Explicit Rigid Body Collision
 */
UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup = VICODynamics)
class VICODYNAMICSPLUGIN_API UVDColliderComponent : public UActorComponent
{
public:
	GENERATED_BODY()

	UVDColliderComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

public:
	/** The Simulation this Component is using */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "VDynamics")
	UVDSimulation* SimulationInstance;

	/** The VDComponent(s) to which the colliders will map to */
	UPROPERTY(EditAnywhere, Category = "VDynamics")
	TArray<FComponentReference> AssociatedVDComponents;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VDynamics")
	uint32 bAddAllComponents : 1;
	/** 
	 * If true, the Convex Collision hulls will be ignored as colliders. Only Boxes, Spheres and Capsules will be considered.
	 * Be careful when turning this off if you have a large number of convex hulls, performance will degrade below using the regular, SceneQuery, collision route.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VDynamics")
	uint32 bIgnoreConvexShapes : 1;

	/** If AddAllComponents if false, specify the components to use as colliders */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "!AddAllComponents"), Category = "VDynamics")
	TArray<FName> SpecificColliders;
};