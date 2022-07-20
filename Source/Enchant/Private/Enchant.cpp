

#include "Enchant.h"

void FEnchantModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FEnchantModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_GAME_MODULE(FEnchantModule, Enchant);
