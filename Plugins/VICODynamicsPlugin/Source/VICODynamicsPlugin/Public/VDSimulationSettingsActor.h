// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "GameFramework/Info.h"

#include "VDSimulationSettingsActor.generated.h"

/** Actor that allows for overriding VICODynamics settngs per-level */
UCLASS(ClassGroup = VDynamics, showcategories = (VDynamics, Rendering, "Utilities|Transformation"))
class VICODYNAMICSPLUGIN_API AVDSimulationSettingsActor : public AInfo
{
	GENERATED_BODY()
	AVDSimulationSettingsActor(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintPure, Category = VDynamics)
	FVDSettings GetSettings() const;


private:
	UPROPERTY(Category = VDynamics, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FVDSettings Settings;
};