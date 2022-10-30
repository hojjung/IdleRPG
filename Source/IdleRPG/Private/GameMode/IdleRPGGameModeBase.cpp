#include "GameMode/IdleRPGGameModeBase.h"
#include "Player/MyPlayerController.h"
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

	UMyGameInstance::Get->OnGameModeStart();
	
	m_Canvas = CreateWidget<UWidgetMainCanvas>(GetWorld(), m_ClassCanvas);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

void AIdleRPGGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UMyGameInstance::Get->Tick(DeltaSeconds);
}

void AIdleRPGGameModeBase::SetFade(FVoidvoid onEnd)
{
	m_Canvas->GetScreenEffect()->ShowFadeOut(1.0f, onEnd);
}

void AIdleRPGGameModeBase::SetHideFade()
{
	if(!m_Canvas)
	{
		return;
	}
	m_Canvas->GetScreenEffect()->HideFadeOut();
}

void AIdleRPGGameModeBase::OpenDeadAlertWidget()
{
	m_Canvas->OpenDeadAlert();
}
