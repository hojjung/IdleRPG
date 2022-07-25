#include "MyGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

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
	
}
