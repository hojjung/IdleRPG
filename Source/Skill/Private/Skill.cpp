#include "Skill.h"


void FSkillModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FSkillModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
IMPLEMENT_GAME_MODULE(FSkillModule,Skill);
