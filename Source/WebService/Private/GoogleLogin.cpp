#include "GoogleLogin.h"

GoogleLogin::~GoogleLogin()
{
	m_OnLoginEnd.Unbind();
}

GoogleLogin::GoogleLogin(FOnLoginEnd dele)
{
	m_OnLoginEnd = dele;
	
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		return;
	}
	ExternalUi->ShowLoginUI(0, false, false, FOnLoginUIClosedDelegate::CreateRaw(this, &GoogleLogin::HandleExternalUIClose));
}

void GoogleLogin::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error)
{
	m_OnLoginEnd.Execute(error.bSucceeded);
}


