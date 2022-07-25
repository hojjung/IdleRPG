#include "Player/MyPlayerPawn.h"
#include "MyAssetManager.h"
#include "Actors/Components/MyNavMovement.h"

AMyPlayerPawn::AMyPlayerPawn(const FObjectInitializer& objInit): Super(objInit)
{
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>(TEXT("CamDissolve00"));
	m_DissolveCam->SetupAttachment(RootComponent);

	m_DissolveCam->m_SocketOffset = FVector(0, 0, -30);
	m_DissolveCam->CameraLagSpeed = 30;
	m_DissolveCam->SetRelativeLocation(FVector(0.f));
	m_DissolveCam->TargetArmLength = 1300; //1375
	m_DissolveCam->SetRelativeRotation(FRotator(-50, -45.f, 0.f)); //-45.f
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_TopCamera"));
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 60.f;

	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
}

void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerPawn::MoveRight);
}
void AMyPlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
void AMyPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	m_PFComp->OnRequestFinished.AddUObject(this, &AMyPlayerPawn::OnRequestMoveDone);

	m_AryIgnores.Reset();

	m_AryIgnores.Add(this);

	m_DissolveCam->SetActive(true);

	m_Asset = UMyAssetManager::Get()->LoadUnitAssetAll(TEXT("Player"),
		FStreamableDelegate::CreateUObject(this, &AMyPlayerPawn::OnLoaded));	
}

void AMyPlayerPawn::OnLoaded()
{
	UUnitEntityAsset* Asset = Cast<UUnitEntityAsset>(m_Asset.Get()->GetLoadedAsset());

	SetEntity(Asset);
}

bool AMyPlayerPawn::IsInputMoving()
{
	return !m_Input.IsZero();
}

void AMyPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsInputMoving())
	{
		ClearStopMoveDelegate();

		m_Movement->SetActive(true);


		FVector Loc = GetCapsule()->GetComponentLocation();

		FVector Delta = (m_DeltaX + m_DeltaY) * 100.0f;

		FVector Dest = Loc + Delta;

		FHitResult Hit;

		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Loc, Loc + Delta, ETraceTypeQuery::TraceTypeQuery3, false,
		                                          m_AryIgnores, EDrawDebugTrace::None, Hit, true))
		{
			Dest = Hit.Location;
		}

		MoveToLocation(Dest, 0);

		m_DeltaX = FVector::ZeroVector;

		m_DeltaY = FVector::ZeroVector;
	}
}

void AMyPlayerPawn::MoveForward(float AxisValue)
{
	m_Input.X = AxisValue;

	if (GetController() && (AxisValue != 0.0f))
	{
		const FRotator Rotation = m_TopCamera->GetComponentRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		m_DeltaX = Direction * AxisValue;
	}
}

void AMyPlayerPawn::MoveRight(float AxisValue)
{
	m_Input.Y = AxisValue;

	if (GetController() && (AxisValue != 0.0f))
	{
		const FRotator Rotation = m_TopCamera->GetComponentRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		m_DeltaY = Direction * AxisValue;
	}
}

void AMyPlayerPawn::OnRequestMoveDone(FAIRequestID id, const FPathFollowingResult& rslt)
{
	if (m_ReqID != id || !rslt.IsSuccess())
	{
		return;
	}
	m_OnRequestDone.ExecuteIfBound();
	m_OnRequestDone.Unbind();
}

void AMyPlayerPawn::RequestAttack()
{
	if (IsMoving())
	{
		return;
	}
	// ACombatUnitPawn* FocusActor = GetFocusedTarget<ACombatUnitPawn>();
	// if (!FocusActor)
	// {
	// 	TryAttack_External();
	// 	return;
	// }
	// RequestInteract(FocusActor, FVoidVoid::CreateUObject(this, &AMyPlayerPawn::TryAttack_External), GetAttackRange());
}

void AMyPlayerPawn::RequestInteract(AActor* target, const FVoidVoid& delegate, float r)
{
	// FPathFollowingRequestResult Result = MoveToActor(target, r);
	//
	// if (Result.Code == EPathFollowingRequestResult::Type::Failed)
	// {
	// 	PRINTF("EPathFollowingRequestResult::Type::Failed");
	// 	return;
	// }
	// if (Result.Code == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	// {
	// 	m_ReqID = FAIRequestID();
	// 	HomingRotateToTarget(0);
	// 	delegate.ExecuteIfBound();
	// 	return;
	// }
	//
	// m_ReqID = Result.MoveId;
	//
	// m_OnRequestDone = delegate;
}

void AMyPlayerPawn::TryAttack_External()
{
	// float Len = TryAttack();
	//
	// if (Len > 0.f && IsSneak())
	// {
	// 	AMonsterPawn* Target = GetFocusedTarget<AMonsterPawn>();
	//
	// 	if (Target && Target->GetFocusedTarget<>() != this)
	// 	{
	// 		m_bIsSneakAttack = true;
	// 	}
	// 	SetSneak();
	// }
	// SetInteracting(false);
}
