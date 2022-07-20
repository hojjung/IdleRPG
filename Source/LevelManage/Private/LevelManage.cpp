#include "LevelManage.h"


void FLevelManageModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FLevelManageModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
IMPLEMENT_GAME_MODULE(FLevelManageModule,LevelManage);
