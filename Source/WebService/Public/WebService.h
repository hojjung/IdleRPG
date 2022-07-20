#pragma once

#include "Modules/ModuleInterface.h"

class  FWebServiceModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};


