#include "Player/MyPlayerController.h"
#include "Entity.h"
#include "Blueprint/UserWidget.h"
#include "Manager/MyCheatManager.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

AMyPlayerController::AMyPlayerController()
{
	CheatClass = UMyCheatManager::StaticClass();
	
	m_CompUseIndex = 0;

	bShowMouseCursor = true;
	//
	SetHidden(false);

	m_bUseFlick = false;

	m_MousePos = FVector2D(0.f);

	//UWidgetMainCanvas
	static ConstructorHelpers::FClassFinder<UWidgetMainCanvas> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/MainGame/MainMenu/WB_MainCanvas.WB_MainCanvas_C'"));
	
	m_ClassCanvas = FoundWW.Class;	
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UWidgetMainCanvas>(GetWorld(), m_ClassCanvas);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}

	SetVirtualJoystickVisibility(true);

	m_Joystick = CurrentTouchInterface;

	UMyGameInstance::Get->SetPlayerCon(this);

	m_PopupTextPool = MakeShareable(new PopupTextPool(30,GetRootComponent())); 
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_PopupTextPool.Reset();
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

void AMyPlayerController::ShowInGameWorldText(BigInt number, const ACombatPawn* interactActor, EDamagePopup dmgPopup)
{
	ShowInGameWorldText(number.ToString(), interactActor, dmgPopup);
}

void AMyPlayerController::ShowInGameWorldText(const FString& stringWant, const ACombatPawn* interactActor,
	EDamagePopup dmgPopup)
{
	m_PopupTextPool->ShowInGameWorldText(stringWant,interactActor->GetActorLocation(),(int)dmgPopup);
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


