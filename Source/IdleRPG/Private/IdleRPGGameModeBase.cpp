#include "IdleRPGGameModeBase.h"
#include "MyGameInstance.h"
#include "MyLib.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerPawn.h"

AIdleRPGGameModeBase::AIdleRPGGameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
	//HUDClass = AMyHUD::StaticClass();
	DefaultPawnClass = AMyPlayerPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;	
}

void AIdleRPGGameModeBase::StartPlay()
{
	Super::StartPlay();

	m_GoogleLogin = MakeShareable<GoogleLogin>(new GoogleLogin(GoogleLogin::FOnLoginEnd::CreateUObject(this, &AIdleRPGGameModeBase::LoginEnd)));

	UMyGameInstance::Get->m_SpawnManager->SpawnUnits(GetWorld(),0,ACombatPawn::FOnDied::CreateUObject(this, &AIdleRPGGameModeBase::TestOnDied),20);
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

void AIdleRPGGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UMyGameInstance::Get->Tick(DeltaSeconds);
}

void AIdleRPGGameModeBase::TestOnDied(const ACombatPawn* p)
{
	
}
