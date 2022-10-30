#include "Manager/MyGameInstance.h"
#include "BUITween.h"
#include "Entity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/StageMode/DefaultStageMode.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	Super::BeginDestroy();

	UBUITween::Shutdown();

	Get = nullptr;

	m_PetManager.Reset();

	m_AvatarManager.Reset();

	m_LevelManager.Reset();

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

	m_LevelManager = MakeShareable(new LevelManager(this));

	m_PlayerGas = MakeShareable(new GAS(GetUniqueID()));
}

void UMyGameInstance::Tick(float d)
{
	m_StageMode->Tick(d);
}

void UMyGameInstance::StartGameMode(EGameMode m, int level, FVoidvoidMulti onLevelChanged)
{
	m_nStageLevel = level;
	
	FName LevelName;

	switch (m)
	{
	case EGameMode::Default:
		{
			m_StageMode = NewObject<UDefaultStageMode>(this);
			
			LevelName = UDefaultStageMode::GetDefaultStage(m_nStageLevel).m_LevelName;
		}
		break;
	case EGameMode::PVP:
		LevelName = TEXT("MapPvp");
	case EGameMode::BoneDragon:
		LevelName = TEXT("MapBoneLayer");
	case EGameMode::Reaper:
		LevelName = TEXT("MapReaper");
	case EGameMode::ChickenRun:
		LevelName = TEXT("MapChicken");
	case EGameMode::Story:
		break;
	}
	LoadMap(LevelName, onLevelChanged);
}

void UMyGameInstance::LoadMap(const FName& levelName, FVoidvoidMulti onDead)
{
	bool ChangeWorldLevel = m_LevelManager->OpenLevel(levelName, onDead);

	if (ChangeWorldLevel)
	{
		GetGameMode()->SetFade();
		return;
	}
	
	GetGameMode()->SetFade(FVoidvoid::CreateUObject(this, &UMyGameInstance::OnLevelMoveFadeEnd));
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	Super::LoadComplete(LoadTime, MapName);
	
	//OnLevelMoveFadeEnd();
}

void UMyGameInstance::OnLevelMoveFadeEnd()
{
	UBUITween::Shutdown();
	
	GetGameMode()->SetHideFade();
	
	m_Player->Revive();
	
	m_LevelManager->OnLoadComplete();
	
	m_StageMode->SetLevel(m_nStageLevel);
	
	m_MapChanged.Broadcast();
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

AIdleRPGGameModeBase* UMyGameInstance::GetGameMode()
{
	return Cast<AIdleRPGGameModeBase>(UGameplayStatics::GetGameMode(this));
}

UStageModeBase* UMyGameInstance::GetStageMode()
{
	return m_StageMode;
}
