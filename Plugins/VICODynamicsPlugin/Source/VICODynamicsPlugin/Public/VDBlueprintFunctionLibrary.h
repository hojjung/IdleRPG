// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VDSimulation.h"
#include "VDBlueprintFunctionLibrary.generated.h"

UCLASS()
class VICODYNAMICSPLUGIN_API UVDBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = VICODynamics)
	static UVDSimulation* GetVICODynamicsSimulationInstance();
};