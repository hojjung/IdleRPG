#include "WebService.h"

#include "Manager/PlayfabManager.h"

void FWebServiceModule::StartupModule()
{
	IModuleInterface::StartupModule();

	m_PlayfabManager = MakeShareable(new PlayfabManager());
}

void FWebServiceModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();

	m_PlayfabManager.Reset();
}

IMPLEMENT_GAME_MODULE(FWebServiceModule,WebService);
