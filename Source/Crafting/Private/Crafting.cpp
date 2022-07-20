#include "Crafting.h"

void FCraftingModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FCraftingModule::ShutdownModule()
{
	IModuleInterface::StartupModule();
}

IMPLEMENT_GAME_MODULE(FCraftingModule, Crafting);