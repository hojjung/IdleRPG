#include "Entity.h"

void FEntityModule::StartupModule()
{
	IModuleInterface::StartupModule();

	m_AvatarManager = MakeShared<AvatarManager>();
}

void FEntityModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();

	m_AvatarManager.Reset();
}
IMPLEMENT_GAME_MODULE(FEntityModule,Entity);
