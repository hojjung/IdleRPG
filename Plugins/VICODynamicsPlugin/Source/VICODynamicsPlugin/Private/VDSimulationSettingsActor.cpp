// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDSimulationSettingsActor.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"

AVDSimulationSettingsActor::AVDSimulationSettingsActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto Defaults = GetDefault<UVICODynamicsSettings>();
	if (Defaults)
		Settings.CopyFrom(Defaults);
}

FVDSettings AVDSimulationSettingsActor::GetSettings() const
{
	return Settings;
}
