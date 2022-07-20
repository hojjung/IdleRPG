#include "Entity.h"

void FEntityModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FEntityModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
IMPLEMENT_GAME_MODULE(FEntityModule,Entity);
