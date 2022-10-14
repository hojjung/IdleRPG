#include "MyGameInstance.h"

#include "Entity.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Manager/GameMode/MyGameModeDefaultStage.h"

UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
	
	Get = nullptr;

	m_PetManager.Reset();

	m_AvatarManager.Reset();
	
	m_SpawnManager.Reset();

	m_GameMode.Reset();
	
	m_Player = nullptr;

	m_PlayerCon = nullptr;
}

void UMyGameInstance::Init()
{
	Super::Init();
	
	UKismetSystemLibrary::ControlScreensaver(false);
	
	Get = this;

	m_PetManager = MakeShareable(new PetManager());

	m_SpawnManager = MakeShareable(new SpawnManager(0));

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
	m_SpawnManager->SpawnUnits(GetWorld(), level, ACombatPawn::FOnDied::CreateRaw(m_GameMode.Get(), &MyGameModeBase::OnMonsterDied) ,20);

	m_OnMapChange.Broadcast(mode, level);
}

FText UMyGameInstance::GetDefaultStageName(int level)
{
	const FStageRow& StageWant = m_SpawnManager->GetStage(level);
	
	float Index = level / 20.0f;

	float UpIndex = 0.0f;

	float DownIndex = 0.0f;

	DownIndex = FMath::Modf(Index, &UpIndex);

	FString Str = FString::Printf(TEXT("%s-%d"), *StageWant.m_StageName.ToString(), (int)DownIndex + 1);

	return FText::FromString(Str);
}

int UMyGameInstance::GetStageLevel()
{
	return m_SpawnManager->GetStageLevel();
}
