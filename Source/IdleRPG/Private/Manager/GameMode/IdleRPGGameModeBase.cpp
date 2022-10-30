#include "Manager/GameMode/IdleRPGGameModeBase.h"

#include "MyLib.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerPawn.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

AIdleRPGGameModeBase::AIdleRPGGameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = nullptr;
	DefaultPawnClass = AMyPlayerPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UWidgetMainCanvas> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/MainGame/MainMenu/WB_MainCanvas.WB_MainCanvas_C'"));
	
	m_ClassCanvas = FoundWW.Class;	
}

void AIdleRPGGameModeBase::StartPlay()
{
	Super::StartPlay();

	m_Canvas = CreateWidget<UWidgetMainCanvas>(GetWorld(), m_ClassCanvas);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}

	m_SpawnManager.Reset();
	
	m_SpawnManager = MakeShareable(new SpawnManager());
}

void AIdleRPGGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_nLevel = -1;
	m_SpawnManager.Reset();
}

void AIdleRPGGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_SpawnManager->Update(DeltaSeconds);
}

void AIdleRPGGameModeBase::SetLevel(int l)
{
	m_nLevel = l;
}

FText AIdleRPGGameModeBase::GetStageName()
{
	return GetStageName(m_nLevel);
}

FText AIdleRPGGameModeBase::GetStageName(int level)
{
	return FText::FromString(TEXT("DefaultStageName"));
}

void AIdleRPGGameModeBase::SetFade(FVoidvoid onEnd)
{
	m_Canvas->GetScreenEffect()->ShowFadeOut(1.0f, onEnd);
}

void AIdleRPGGameModeBase::SetHideFade()
{
	m_Canvas->GetScreenEffect()->HideFadeOut();
}
