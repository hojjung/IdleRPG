#include "ItemInventory.h"


void FItemInventoryModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FItemInventoryModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

IMPLEMENT_GAME_MODULE(FItemInventoryModule,ItemInventory);
