#pragma once

#include "Modules/ModuleInterface.h"

class PlayfabManager;
DECLARE_DELEGATE_TwoParams(FOnTextAlert, FText, FLinearColor);


class DeleterNot
{
public:
	void operator()(void*) {}
};

class  FWebServiceModule: public IModuleInterface
{
public:
	TSharedPtr<PlayfabManager> m_PlayfabManager;
	
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};


