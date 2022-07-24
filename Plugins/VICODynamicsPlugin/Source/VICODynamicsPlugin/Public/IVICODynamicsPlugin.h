// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#include "Simulation.h"
#include "VDSimulation.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVICODynamics, All, All);

/**
 * The public interface to this module
 */
class IVICODynamicsPlugin : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IVICODynamicsPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked<IVICODynamicsPlugin>("VICODynamicsPlugin");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("VICODynamicsPlugin");
	}

	UVDSimulation* VDSimulation;
	bool Released;
};

