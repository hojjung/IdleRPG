#pragma once

#include "Modules/ModuleInterface.h"

class  FEnchantModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};



