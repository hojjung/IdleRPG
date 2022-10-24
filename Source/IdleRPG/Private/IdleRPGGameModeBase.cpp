#include "IdleRPGGameModeBase.h"
#include "MyLib.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerPawn.h"

AIdleRPGGameModeBase::AIdleRPGGameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
	//HUDClass = AMyHUD::StaticClass();
	DefaultPawnClass = AMyPlayerPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UWidgetMainCanvas> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/MainGame/MainMenu/WB_MainCanvas.WB_MainCanvas_C'"));
	
	m_ClassCanvas = FoundWW.Class;	
}

void AIdleRPGGameModeBase::StartPlay()
{
	Super::StartPlay();

	m_GoogleLogin = MakeShareable<GoogleLogin>(new GoogleLogin(GoogleLogin::FOnLoginEnd::CreateUObject(this, &AIdleRPGGameModeBase::LoginEnd)));

	UMyGameInstance::Get->StartGameMode(EGameMode::Default,0);

	m_Canvas = CreateWidget<UWidgetMainCanvas>(GetWorld(), m_ClassCanvas);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

void AIdleRPGGameModeBase::LoginEnd(bool bSuccess)
{
	if(bSuccess)
	{
		PRINTF("Succcccccc");
	}
	else
	{
		PRINTF("Fafffffff");
	}

	m_GoogleLogin.Reset();
}

void AIdleRPGGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UMyGameInstance::Get->Tick(DeltaSeconds);
}

void AIdleRPGGameModeBase::TestOnDied(const ACombatPawn* p)
{
	
}
