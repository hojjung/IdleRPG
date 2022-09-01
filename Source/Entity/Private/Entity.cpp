#include "Entity.h"

#include "Avatar/AvatarManager.h"

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

AvatarManager* FEntityModule::GetAvatarManager()
{
	return m_AvatarManager.Get();
}

IMPLEMENT_GAME_MODULE(FEntityModule, Entity);
