#pragma once

#include "Modules/ModuleInterface.h"


class  FEntityModule: public IModuleInterface
{
	
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};