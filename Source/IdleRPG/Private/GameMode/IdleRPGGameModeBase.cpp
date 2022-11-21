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

	static ConstructorHelpers::FClassFinder<UWidgetScreenFadeCanvas> FoundWW2(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/MainGame/MainMenu/WB_ScreenFade.WB_ScreenFade_C'"));
	
	m_ClassCanvasFade = FoundWW2.Class;
}

void AIdleRPGGameModeBase::StartPlay()
{
	Super::StartPlay();

	UMyGameInstance::Get->OnGameModeStart();
	
	m_Canvas = CreateWidget<UWidgetMainCanvas>(GetWorld(), m_ClassCanvas);

	m_Canvas->AddToViewport();
	
	m_CanvasFade = CreateWidget<UWidgetScreenFadeCanvas>(GetWorld(), m_ClassCanvasFade);

	m_CanvasFade->AddToViewport(9999);
	
	UMyGameInstance::Get->TryAddModeWidget();
	
	UMyGameInstance::Get->TryOpenDeadAlert();

	AMyPlayerPawn* Pl = Cast<AMyPlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	UMyGameInstance::Get->SetPlayerPawn(Pl);
	
	UMyGameInstance::Get->m_AvatarManager.Get()->SetEquippedAvatar();
}

void AIdleRPGGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UMyGameInstance::Get->Tick(DeltaSeconds);
}

void AIdleRPGGameModeBase::SetFade(FVoidvoid onEnd)
{
	m_CanvasFade->GetScreenEffect()->ShowFadeOut(0.6f, onEnd);
}

void AIdleRPGGameModeBase::SetHideFade()
{
	m_CanvasFade->GetScreenEffect()->HideFadeOut();
}

void AIdleRPGGameModeBase::OpenDeadAlertWidget()
{
	m_Canvas->OpenDeadAlert();
}
