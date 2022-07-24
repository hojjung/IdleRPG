#include "IdleRPGGameModeBase.h"

void AIdleRPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	m_GoogleLogin = MakeShareable<GoogleLogin>(new GoogleLogin(GoogleLogin::FOnLoginEnd::CreateUObject(this, &AIdleRPGGameModeBase::LoginEnd))); 
}

void AIdleRPGGameModeBase::LoginEnd(bool bSuccess)
{
	if(bSuccess)
	{
		PRINTF("Succcccccc");
	}
	else
	{
		PRINTF("Fafffffff");
	}

	m_GoogleLogin.Reset();
}
