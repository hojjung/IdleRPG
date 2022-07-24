// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDBlueprintFunctionLibrary.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"

UVDBlueprintFunctionLibrary::UVDBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UVDSimulation* UVDBlueprintFunctionLibrary::GetVICODynamicsSimulationInstance()
{
	IVICODynamicsPlugin& VDPluginModule = IVICODynamicsPlugin::Get();

	if (VDPluginModule.Released) return nullptr;

	auto Instance = VDPluginModule.VDSimulation;
	return Instance;
}