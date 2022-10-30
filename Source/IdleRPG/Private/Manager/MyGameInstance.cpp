#include "Manager/MyGameInstance.h"
#include "BUITween.h"
#include "Entity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/GameMode/DefaultGameMode.h"
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

	m_AryStage.Reserve(100);

	UStageTable::GetDefaultStage->GetAllRows<FStageRow>("", m_AryStage);

	UKismetSystemLibrary::ControlScreensaver(false);

	Get = this;

	m_PetManager = MakeShareable(new PetManager());

	m_AvatarManager = MakeShareable(new AvatarManager());

	m_GoldManager = MakeShareable(new GoldManager());

	m_LevelManager = MakeShareable(new LevelManager(this));

	m_PlayerGas = MakeShareable(new GAS(GetUniqueID()));
}

void UMyGameInstance::OnMonsterDead(AMonsterPawn* target)
{
	GetGameMode()->OnMonsterDead(target);
}

void UMyGameInstance::OnPlayerDead(AMyPlayerPawn* target)
{
	GetGameMode()->OnPlayerDead(target);
}

void UMyGameInstance::OnPlayerAnimEnd(AMyPlayerPawn* target)
{
	GetGameMode()->OnPlayerAnimEnd(target);
}

void UMyGameInstance::OnMonsterAnimEnd(AMonsterPawn* target)
{
	GetGameMode()->OnMonsterAnimEnd(target);
}

void UMyGameInstance::StartGameMode(EGameMode m, int level, FVoidvoid onLevelChanged)
{
	m_nStageLevel = level;
	
	FName LevelName;

	switch (m)
	{
	case EGameMode::Default:
		{
			LevelName = ADefaultGameMode::GetDefaultStage(m_nStageLevel).m_LevelName;
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

void UMyGameInstance::LoadMap(const FName& levelName, FVoidvoid onDead)
{
	m_LevelManager->m_OnLevelLoadComplete = onDead;

	bool MapDiffrent = m_LevelManager->OpenLevel(levelName);

	if (MapDiffrent)
	{
		m_MapChanged.Broadcast();
		GetGameMode()->SetFade();
		return;
	}
	
	GetGameMode()->SetFade(FVoidvoid::CreateUObject(this, &UMyGameInstance::OnLevelMoveFadeEnd));
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	Super::LoadComplete(LoadTime, MapName);
	UBUITween::Shutdown();
	m_LevelManager->OnLoadComplete(MapName);
	GetGameMode()->SetLevel(m_nStageLevel);
}

void UMyGameInstance::OnLevelMoveFadeEnd()
{
	GetGameMode()->SetHideFade();
	
	m_Player->Revive();
	
	m_LevelManager->m_OnLevelLoadComplete.ExecuteIfBound();
	
	GetGameMode()->SetLevel(m_nStageLevel);
	
	m_MapChanged.Broadcast();
}

FText UMyGameInstance::GetStageName()
{
	return GetGameMode()->GetStageName();
}

int UMyGameInstance::GetStageLevel()
{
	return GetGameMode()->GetLevel();
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
