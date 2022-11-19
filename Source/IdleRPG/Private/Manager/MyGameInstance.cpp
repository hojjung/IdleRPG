#include "Manager/MyGameInstance.h"
#include "BUITween.h"
#include "Entity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/StageMode/DefaultStageMode.h"
#include "Manager/StageMode/RaidStageMode.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	Super::BeginDestroy();

	UBUITween::CompleteAll();
	UBUITween::Shutdown();

	Get = nullptr;

	m_PetManager.Reset();

	m_AvatarManager.Reset();

	m_LevelManager.Reset();

	m_GoldManager.Reset();

	m_Player = nullptr;

	m_PlayerCon = nullptr;
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	Super::LoadComplete(LoadTime, MapName);

	UBUITween::CompleteAll();
	UBUITween::Shutdown();
}

void UMyGameInstance::Init()
{
	Super::Init();

	UKismetSystemLibrary::ControlScreensaver(false);

	Get = this;

	m_bIsPlayerDead = false;

	m_PetManager = MakeShareable(new PetManager());

	m_AvatarManager = MakeShareable(new AvatarManager());

	m_GoldManager = MakeShareable(new GoldManager());

	m_ExpManager = MakeShareable(new ExpManager());

	m_LevelManager = MakeShareable(new LevelManager(this));

	m_PlayerGas = MakeShareable(new GAS(GetUniqueID()));

	m_StageMode = NewObject<UDefaultStageMode>(this);

	m_StageMode->SetLevel(m_nStageLevel);

	
}

void UMyGameInstance::OnGameModeStart()
{
	if(m_StageMode)
	{
		m_StageMode->Clear();
	}
	if(m_Content == nullptr)
	{
		m_StageMode = NewObject<UDefaultStageMode>(this);
	}
	else
	{
		switch (m_Content->m_GameMode)
		{
		case EGameMode::PVP:
			{
			
			}
			break;
		case EGameMode::BoneDragon:
		case EGameMode::Reaper:
			{
				URaidStageMode* StageMode = NewObject<URaidStageMode>(this);
			
				StageMode->SetContentData(*m_Content);
			
				m_StageMode = StageMode;
			}
			break;
		case EGameMode::Elf:
			{
			
			}
			break;
		case EGameMode::Japan:
			{
			
			}
			break;
		case EGameMode::Castle:
			{
			
			}
			break;
		}
	}
	m_StageMode->SetLevel(m_nStageLevel);
	
	m_StageMode->StartGame();
}

void UMyGameInstance::Tick(float d)
{
	m_StageMode->Tick(d);
}

//레벨로드 따로
//스포닝 따로
void UMyGameInstance::StartGameMode(int level, const FContentDataRow* contentData)
{
	m_nStageLevel = level;

	m_Content = contentData;
	
	FName LevelName;

	if(contentData == nullptr)
	{
		LevelName = UDefaultStageMode::GetDefaultStage(m_nStageLevel).m_LevelName;

		m_nDefaultStageLevel = m_nStageLevel;
	}
	else
	{
		LevelName = contentData->m_MapName;
	}
	
	LoadMap(LevelName);
}

void UMyGameInstance::LoadMap(const FName& levelName)
{
	bool IsPlayerDead = m_bIsPlayerDead;
	
	bool ChangeWorldLevel = m_LevelManager->IsNeedChangeMap(levelName);

	if(ChangeWorldLevel)
	{
		if(IsPlayerDead)
		{
			m_LevelManager->OpenLevel(levelName);
		}
		else
		{
			GetGameMode()->SetFade(FVoidvoid::CreateWeakLambda(this, [=]()
			{
				m_LevelManager->OpenLevel(levelName);
			}));
		}
	}
	else
	{
		if(IsPlayerDead)
		{
			TryOpenDeadAlert();
			OnLevelMoveFadeEnd();
		}
		else
		{
			GetGameMode()->SetFade(FVoidvoid::CreateUObject(this, &UMyGameInstance::OnLevelMoveFadeEnd));
		}
	}
}

void UMyGameInstance::OnLevelMoveFadeEnd()
{
	UBUITween::CompleteAll();
	UBUITween::Shutdown();
	
	GetGameMode()->SetHideFade();
	
	m_Player->Revive();
	
	OnGameModeStart();
	
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

void UMyGameInstance::TryOpenDeadAlert()
{
	if(UMyGameInstance::Get->m_bIsPlayerDead)
	{
		GetGameMode()->OpenDeadAlertWidget();
		
		UMyGameInstance::Get->m_bIsPlayerDead = false;
	}
}

void UMyGameInstance::TryAddModeWidget()
{
	GetStageMode()->TryAddModeWidget();
}

int UMyGameInstance::GetDefaultStageLevel()
{
	return m_nDefaultStageLevel;
}

void UMyGameInstance::SetFade(const FVoidvoid& voidvoid)
{
	GetStageMode()->SetFade(voidvoid);
}

void UMyGameInstance::HideFadeOut()
{
	GetStageMode()->HideFadeOut();
}
