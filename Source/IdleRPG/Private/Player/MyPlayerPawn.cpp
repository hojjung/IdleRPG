#include "Player/MyPlayerPawn.h"

#include "Entity.h"
#include "IdleRPG.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "NavigationSystem.h"
#include "Actors/Components/MyNavMovement.h"
#include "Pet/PetPawn.h"
#include "Player/MyFlockSteering.h"
#include "Player/PlayerSensor.h"

AMyPlayerPawn::AMyPlayerPawn(const FObjectInitializer& objInit): Super(objInit)
{
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_DissolveCam = CreateDefaultSubobject<UCameraDissolve>(TEXT("CamDissolve00"));
	m_DissolveCam->SetupAttachment(RootComponent);
	m_DissolveCam->m_TargetOffset = FVector(0, 0, 0);//-30
	m_DissolveCam->CameraLagSpeed = 30;
	m_DissolveCam->TargetArmLength = 3700; //1375
	m_DissolveCam->SetRelativeRotation(FRotator(-45, -45.f, 0.f)); //-45.f
	
	m_TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_TopCamera"));
	m_TopCamera->SetupAttachment(m_DissolveCam);
	m_TopCamera->FieldOfView = 15.f;

	m_Movement->MaxSpeed = FGlobalVariable::HERO_DEFAULT_SPEED;

	m_bCanMoveInSkill = false;
	m_bIsSkillUsing = false;
	m_PFComp->SetAcceptanceRadius(34);

	m_SwingSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio03"));
	m_SwingSoundComp->SetupAttachment(RootComponent);
	m_SwingSoundComp->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<USoundBase> FoundSwingSound(
	TEXT("SoundCue'/Game/Sound/Use/SwordSwing.SwordSwing'"));

	m_SwingSoundComp->SetSound(FoundSwingSound.Object);
	//SoundCue'/Game/Sound/Use/SwordSwing.SwordSwing'
}

void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerPawn::MoveRight);
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

	UMyGameInstance::Get->SetPlayerPawn(this);

	m_AryIgnores.Reset();

	m_AryIgnores.Add(this);

	m_DissolveCam->SetActive(true);

	m_Sensor = MakeShareable(new PlayerSensor(this));

	m_Fsm = MakeShareable(new PlayerFSM(this));

	SetAtkRange(200);
	
	UMyGameInstance::Get->m_AvatarManager.Get()->SetEquippedAvatar();
}

void AMyPlayerPawn::SetEntity(const UUnitAsset* asset)
{
	Super::SetEntity(asset);

	if(m_AddVisual)
	{
		m_AddVisual->Hide();
	}
	
	m_AddVisual = NewObject<UAvatarAddtionalVisuals>(this);
	m_AddVisual->Init(GetSkMesh());

	int Index = 0;
	for(const auto& Attach :  asset->m_AryAttachments)
	{
		m_AddVisual->SpawnAttachment(asset->m_ArySocketAttachments[Index++], Attach.Get());
	}
}

bool AMyPlayerPawn::IsInputMoving()
{
	return !m_Input.IsZero();
}

void AMyPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_Sensor->Update(DeltaSeconds);
	
	m_Fsm->Update(DeltaSeconds);

	if(m_bIsSkillUsing && !m_bCanMoveInSkill)
	{
		return;
	}
	if (IsInputMoving())
	{
		SetFocusedTarget(nullptr);
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

	m_AddVisual->TickWind(IsMoving(), DeltaSeconds);
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

void AMyPlayerPawn::StopAnimMontage()
{
	Super::StopAnimMontage();

	m_AddVisual->SetAttacking(-1);
}

void AMyPlayerPawn::TryAttack_External()
{
	float t = TryAttack(3.5f);

	if(t > 0)
	{
		m_AddVisual->SetAttacking(t);
	}
}

bool AMyPlayerPawn::IsAlive()
{
	return true;
}

float AMyPlayerPawn::PlayBaseAttackAnim(float rate)
{
	m_SwingSoundComp->Play();
	
	return Super::PlayBaseAttackAnim(rate);
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

void AMyPlayerPawn::SetCameraCenter()
{
	m_DissolveCam->m_TargetOffset = FVector(0,0,0);
}

void AMyPlayerPawn::SetCameraTop()
{
	m_DissolveCam->m_TargetOffset = FVector(-440,440,0);
}

void AMyPlayerPawn::SetPet(const UUnitAsset* pet)
{
	UnEquipPet();

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FVector Loc = Nav->GetRandomPointInNavigableRadius(GetWorld(),GetActorLocation(),400);

	m_Pet = GetWorld()->SpawnActor<APetPawn>(APetPawn::StaticClass(),Loc,FRotator(0),Param);

	m_Pet->SetPetEntity(pet);
}

void AMyPlayerPawn::UnEquipPet()
{
	if(m_Pet.Get())
	{
		m_Pet->Destroy();
	}
}

bool AMyPlayerPawn::IsInCombat()
{
	return m_Fsm->GetState() == PlayerFSM::EFSM::Combat;
}
