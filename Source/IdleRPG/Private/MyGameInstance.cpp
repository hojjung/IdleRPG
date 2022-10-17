#include "MyGameInstance.h"

#include "BUITween.h"
#include "BUITweenInstance.h"
#include "Entity.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Manager/GameMode/MyGameModeDefaultStage.h"

UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	Super::BeginDestroy();

	UBUITween::Shutdown();
	
	Get = nullptr;

	m_PetManager.Reset();

	m_AvatarManager.Reset();
	
	m_SpawnManager.Reset();

	m_GameMode.Reset();

	m_GoldManager.Reset();
	
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

	m_GoldManager = MakeShareable(new GoldManager());

	m_PlayerGas = MakeShareable(new GAS(GetUniqueID()));
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	UBUITween::Shutdown();
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
	m_SpawnManager->SpawnUnits(GetWorld(), level,20);

	m_OnMapChange.Broadcast(mode, level);
}

void UMyGameInstance::OnMonsterDead(AMonsterPawn* target)
{
	m_GameMode->OnMonsterDead(target);
}

void UMyGameInstance::OnMonsterAnimEnd(AMonsterPawn* target)
{
	m_GameMode->OnMonsterAnimEnd(target);
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

void UMyGameInstance::SetPlayerPawn(AMyPlayerPawn* p)
{
	m_PlayerGas->Restart();
	
	m_Player = p;

	m_Player->SetGas(m_PlayerGas);
}

AMyPlayerPawn* UMyGameInstance::GetPlayerPawn()
{
	return m_Player.Get();
}

void UMyGameInstance::SetPlayerCon(AMyPlayerController* p)
{
	m_PlayerCon = p;
}

AMyPlayerController* UMyGameInstance::GetPlayerCon()
{
	return m_PlayerCon.Get();
}
