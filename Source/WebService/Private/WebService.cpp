#include "WebService.h"

void FWebServiceModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FWebServiceModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_GAME_MODULE(FWebServiceModule,WebService);
