#include "MyGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/GameMode/MyGameModeDefaultStage.h"

UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
	
	Get = nullptr;
	
	m_SpawnManager.Reset();
	
	//m_Player = nullptr;

	//m_PlayerCon = nullptr;
}

void UMyGameInstance::Init()
{
	Super::Init();
	
	UKismetSystemLibrary::ControlScreensaver(false);
	
	Get = this;

	m_SpawnManager = MakeShareable(new SpawnManager());

	m_AvatarManager = MakeShareable(new AvatarManager());
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	// if(m_LevelMoveManager->IsGameStart())
	// {
	// 	m_LevelMoveManager->OnOpenWorldLevelComplete();
	// }
}

void UMyGameInstance::Tick(float deltaTime)
{
	m_SpawnManager->Update(deltaTime);
}

void UMyGameInstance::StartGameMode(EGameMode mode, int level)
{
	m_GameMode.Reset();
	switch (mode)
	{
	case EGameMode::Default:
		m_GameMode = MakeShareable(new MyGameModeDefaultStage());
		break;
	} 
	
	m_SpawnManager->Clear();
	m_SpawnManager->SpawnUnits(GetWorld(), 0, ACombatPawn::FOnDied::CreateRaw(m_GameMode.Get(), &MyGameModeBase::OnMonsterDied) ,20);
}
