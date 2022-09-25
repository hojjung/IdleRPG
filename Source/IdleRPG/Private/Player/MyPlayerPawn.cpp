#include "Player/MyPlayerPawn.h"

#include "Entity.h"
#include "MyAssetManager.h"
#include "Actors/Components/MyNavMovement.h"
#include "Player/MyFlockSteering.h"
#include "Player/PlayerSensor.h"

AMyPlayerPawn::AMyPlayerPawn(const FObjectInitializer& objInit): Super(objInit)
{
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>(TEXT("CamDissolve00"));
	m_DissolveCam->SetupAttachment(RootComponent);

	m_DissolveCam->m_SocketOffset = FVector(0, 0, -30);
	m_DissolveCam->CameraLagSpeed = 30;
	m_DissolveCam->SetRelativeLocation(FVector(0.f));
	m_DissolveCam->TargetArmLength = 1000; //1375
	m_DissolveCam->SetRelativeRotation(FRotator(-60, -45.f, 0.f)); //-45.f
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_TopCamera"));
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 45.f;

	m_AryTargetingObjectType.Reset();
	m_AryTargetingObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);

	m_Movement->MaxSpeed = 400.f;

	m_bCanMoveInSkill = false;
	m_bIsSkillUsing = false;

	m_Cape = CreateDefaultSubobject<UCapeComponent>(TEXT("m_Cape"));
	m_Cape->SetupAttachment(RootComponent, TEXT("Cape"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CapeMeshTest(TEXT("StaticMesh'/Game/CharacterMesh/Cape/PolyCapes/ST_RedCloak.ST_RedCloak'"));
	m_Cape->SetStaticMesh(CapeMeshTest.Object);

	m_PFComp->SetAcceptanceRadius(34);
}

void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerPawn::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed,this, &AMyPlayerPawn::RequestAttack);
	//PlayerInputComponent->BindTouch()
}
void AMyPlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_Sensor.Reset();

	m_Fsm.Reset();
}
void AMyPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	m_PFComp->OnRequestFinished.AddUObject(this, &AMyPlayerPawn::OnRequestMoveDone);

	m_AryIgnores.Reset();

	m_AryIgnores.Add(this);

	m_DissolveCam->SetActive(true);

	FPrimaryAssetId ID = FPrimaryAssetId(TEXT("Unit"),TEXT("DeathKnight01"));
	UMyAssetManager::Get()->LoadUnitAssetMeshOnly( ID,
		FStreamableDelegate::CreateUObject(this, &AMyPlayerPawn::OnLoaded, ID));

	m_Sensor = MakeShareable(new PlayerSensor(this));

	m_Fsm = MakeShareable(new PlayerFSM(this));

	SetAtkRange(200);
}

void AMyPlayerPawn::OnLoaded(FPrimaryAssetId id)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	
	UUnitAsset* Asset = Cast<UUnitAsset>(Manager->GetPrimaryAssetObject(id));

	SetEntity(Asset);

	FAttachmentTransformRules Rule (EAttachmentRule::SnapToTarget, true);
	m_Cape->AttachToComponent(GetSkMesh(), Rule, TEXT("Cape"));
}

bool AMyPlayerPawn::IsInputMoving()
{
	return !m_Input.IsZero();
}

void AMyPlayerPawn::OnTickAlive(float DeltaSeconds)
{
	Super::OnTickAlive(DeltaSeconds);

	m_Sensor->Update(DeltaSeconds);
	m_Fsm->Update(DeltaSeconds);

	if(m_bIsSkillUsing && !m_bCanMoveInSkill)
	{
		return;
	}
	if (IsInputMoving())
	{
		if(!m_bCanMoveInSkill)
		{
			StopAnimMontage();
		}
		ClearStopMoveDelegate();
		
		m_Movement->SetActive(true);

		FVector Loc = RootComponent->GetComponentLocation();

		FVector Delta = (m_DeltaX + m_DeltaY) * 100.0f;

		FVector Dest = Loc + Delta;

		MoveToLocation(Dest, 0);
		
		m_DeltaX = FVector::ZeroVector;

		m_DeltaY = FVector::ZeroVector;
	}

	if(IsMoving())
	{
		m_Cape->SetWindPower(30);
	}
	else
	{
		m_Cape->SetWindPower(10);
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

void AMyPlayerPawn::TryAttack_External()
{
	TryAttack();
}

void AMyPlayerPawn::RequestAttack()
{
	if (m_bIsSkillUsing || IsMoving())
	{
		return;
	}
	
	ACombatPawn* FocusActor = GetFocusedTarget();
	
	if (!FocusActor)
	{
		TryAttack_External();
		
		return;
	}
	RequestInteract(FocusActor, FVoidVoid::CreateUObject(this, &AMyPlayerPawn::TryAttack_External), GetAttackRange());
}

void AMyPlayerPawn::RequestInteract(AActor* target, const FVoidVoid& delegate, float r)
{
	FPathFollowingRequestResult Result = MoveToActor(target, r);
	
	if (Result.Code == EPathFollowingRequestResult::Type::Failed)
	{
		return;
	}
	if (Result.Code == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		m_ReqID = FAIRequestID();
		HomingRotateToTarget(0);
		delegate.ExecuteIfBound();
		return;
	}
	
	m_ReqID = Result.MoveId;
	
	m_OnRequestDone = delegate;
}

bool AMyPlayerPawn::IsAlive()
{
	return true;
}

void AMyPlayerPawn::SetSkillUsing(bool b)
{
	m_bIsSkillUsing = b;
}

bool AMyPlayerPawn::GetSkillUsing()
{
	return m_bIsSkillUsing;
}

void AMyPlayerPawn::SetCanMoveInSkill(bool b)
{
	m_bCanMoveInSkill = b;
}

bool AMyPlayerPawn::CanMoveInSkill()
{
	return m_bCanMoveInSkill;
}