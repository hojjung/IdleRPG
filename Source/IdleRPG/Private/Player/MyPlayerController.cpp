#include "Player/MyPlayerController.h"
#include "Entity.h"

AMyPlayerController::AMyPlayerController()
{
	//CheatClass = UMyCheatManager::StaticClass();
	
	m_CompUseIndex = 0;

	bShowMouseCursor = true;
	//
	SetHidden(false);

	m_bUseFlick = false;

	m_MousePos = FVector2D(0.f);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//CreateIGWC(10);

	SetVirtualJoystickVisibility(true);

	m_Joystick = CurrentTouchInterface;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &AMyPlayerController::OpenExitPanel);
	InputComponent->BindAction("MouseClick", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnTouchPressed);

	InputComponent->BindAction("MouseClick", EInputEvent::IE_Pressed, this, &AMyPlayerController::OnPressed);
	InputComponent->BindAction("MouseClick", EInputEvent::IE_Released, this, &AMyPlayerController::OnReleased);
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!m_bUseFlick)
	{
		return;
	}
	FVector2D OldPos = m_MousePos;

	GetMousePosition(m_MousePos.X,m_MousePos.Y);

	FVector2D Delta = (m_MousePos - OldPos) * 2.f;

	m_OnFlick.Broadcast(Delta);
}

void AMyPlayerController::OpenExitPanel()
{
	// UWidgetConfirmPanel::FOnClick Cancel;
	//
	// UWidgetConfirmPanel::FOnClick Confirm = UWidgetConfirmPanel::FOnClick::CreateUObject(this,&AMyPlayerController::ExitGame); 
	//
	// FString DescStrF = NSLOCTEXT("AMyPlayerController","OnExit","정말로 게임을 종료 하시겠습니까?").ToString();
	//
	// UMyLib::GetCanvas()->GetConfirmPanel()->SetConfirmPanel(DescStrF,Cancel,Confirm);
}

void AMyPlayerController::BackToSelectMenu()
{
	PRINTF("Continue StartMenu");
	ClientForceGarbageCollection();
	UGameplayStatics::OpenLevel(GetWorld(), "StartMenu");
}

void AMyPlayerController::ExitGame()
{
	PRINTF("TryExit");

	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void AMyPlayerController::OnPressed()
{
	m_bUseFlick = true;
	GetMousePosition(m_MousePos.X,m_MousePos.Y);
}

void AMyPlayerController::OnReleased()
{
	m_bUseFlick = false;
	m_MousePos = FVector2D(0.f);
}

void AMyPlayerController::OnTouchPressed()
{
	FHitResult Hit;
    	
	GetHitResultUnderCursor(ECC_EngineTraceChannel4, false, Hit);
    
	if(!Hit.bBlockingHit)
	{
		return ;
	}

	m_OnTouch.Broadcast(Hit);
}

void AMyPlayerController::EnableJoystick(bool b)
{
	if(b)
	{
		ActivateTouchInterface(m_Joystick);
	}
	else
	{
		ActivateTouchInterface(nullptr);
	}
}


