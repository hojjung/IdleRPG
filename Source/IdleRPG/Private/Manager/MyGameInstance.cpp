#include "Manager/MyGameInstance.h"
#include "BUITween.h"
#include "Entity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/GameMode/MyGameModeDefaultStage.h"
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

void UMyGameInstance::OnMonsterAnimEnd(AMonsterPawn* target)
{
	GetGameMode()->OnMonsterAnimEnd(target);
}

void UMyGameInstance::StartGameMode(EGameMode m, int level)
{
	m_nStageLevel = level;
	
	FName LevelName;
	
	switch (m)
	{
	case EGameMode::Default:
		{
			int Stage = level % m_AryStage.Num();

			LevelName = m_AryStage[Stage]->m_LevelName;		
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
	LoadMap(LevelName);
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
	return GetGameMode()->GetStageName();
}

int UMyGameInstance::GetStageLevel()
{
	return m_nStageLevel;
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
