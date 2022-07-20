#pragma once

#include "Engine.h"
#include "Modules/ModuleInterface.h"

class OnDataTableChanged;
//DECLARE_LOG_CATEGORY_EXTERN (FFantasySurvivalEditorModule, All, All);
class  FIdleRPGEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	TSharedPtr<OnDataTableChanged> m_OnDataTableChanged;

	void RefreshPlayerSkillData();
};


