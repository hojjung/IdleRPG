#include "Manager/MyGameInstance.h"
#include "BUITween.h"
#include "Entity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/GameMode/MyGameModeDefaultStage.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	m_GameMode.Reset();
	
	Super::BeginDestroy();

	UBUITween::Shutdown();
	
	Get = nullptr;

	m_PetManager.Reset();

	m_AvatarManager.Reset();
	

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

	m_AvatarManager = MakeShareable(new AvatarManager());

	m_GoldManager = MakeShareable(new GoldManager());

	m_LevelManager= MakeShareable(new LevelManager(this));

	m_PlayerGas = MakeShareable(new GAS(GetUniqueID()));
}



void UMyGameInstance::Tick(float deltaTime)
{
	m_GameMode->Update(deltaTime);
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
	
	m_GameMode->SetLevel(level);
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	UBUITween::Shutdown();
	
	m_LevelManager->OnLoadComplete(MapName);
}

void UMyGameInstance::OnMonsterDead(AMonsterPawn* target)
{
	m_GameMode->OnMonsterDead(target);
}

void UMyGameInstance::OnPlayerDead(AMyPlayerPawn* target)
{
	m_GameMode->OnPlayerDead(target);
}

void UMyGameInstance::OnMonsterAnimEnd(AMonsterPawn* target)
{
	m_GameMode->OnMonsterAnimEnd(target);
}

void UMyGameInstance::LoadMap(const FName& levelName, FVoidvoid onLevelChanged)
{
	m_LevelManager->m_OnLevelLoadComplete = onLevelChanged;
	
	bool MapDiffrent = m_LevelManager->OpenLevel(levelName);

	if(MapDiffrent)
	{
		return;
	}
	onLevelChanged.ExecuteIfBound();
	m_Player->Revive();
}

FText UMyGameInstance::GetStageName()
{
	return m_GameMode->GetStageName();
}

int UMyGameInstance::GetStageLevel()
{
	return m_GameMode->GetLevel();
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
