#include "Player/MyPlayerPawn.h"
#include "Entity.h"
#include "IdleRPG.h"
#include "MyAssetManager.h"
#include "NavigationSystem.h"
#include "Actors/Components/MyNavMovement.h"
#include "Manager/MyGameInstance.h"
#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfoComp.h"
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
	m_TopCamera->SetupAttachment(m_DissolveCam, USpringArmComponent::SocketName);
	m_TopCamera->FieldOfView = 15.f;

	m_Movement->SetMaxSpeed(FGlobalVariable::HERO_DEFAULT_SPEED);

	m_bCanMoveInSkill = false;
	m_bIsSkillUsing = false;
	m_PFComp->SetAcceptanceRadius(34);

	m_SwingSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio03"));
	m_SwingSoundComp->SetupAttachment(RootComponent);
	m_SwingSoundComp->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<USoundBase> FoundSwingSound(
	TEXT("SoundCue'/Game/Sound/Use/SwordSwing.SwordSwing'"));

	m_SwingSoundComp->SetSound(FoundSwingSound.Object);
	
	m_PawnInfo = CreateDefaultSubobject<UWidgetPawnInfoComp>(TEXT("PawnInfoWidget"));
	m_PawnInfo->SetupAttachment(RootComponent);
	m_PawnInfo->SetVisibility(false);
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundHpBar(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/Comp/WB_PawnInfo.WB_PawnInfo_C'"));
	m_PawnInfo->SetWidgetClass(FoundHpBar.Class);
	//m_PawnInfo->SetDrawSize(FVector2D(150.f, 22.f));
	m_PawnInfo->SetDrawAtDesiredSize(true);
	
	FVector2D Pivot(0.5f, 0.5f);
	m_PawnInfo->SetPivot(Pivot);
	m_PawnInfo->SetWidgetSpace(EWidgetSpace::Screen);
	m_PawnInfo->SetRelativeLocation(FVector(0, 0, 130));
	m_PawnInfo->SetCanEverAffectNavigation(false);

	m_fAtkDur = 1.0f;
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

void AMyPlayerPawn::Revive()
{
	Super::Revive();
	StopAnimMontage();
	m_PawnInfo->SetVisibility(true);
	m_PawnInfo->SetPawnInfo(this);
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
	
	float Z =  m_BodyMesh->Bounds.BoxExtent.Z;

	m_PawnInfo->SetRelativeLocation(FVector(0,0,Z));

	m_PawnInfo->SetPawnInfo(this);
	//
	m_AryIgnores.Reset();

	m_AryIgnores.Add(this);

	m_DissolveCam->SetActive(true);

	m_Sensor = MakeShareable(new PlayerSensor(this));

	m_Fsm = MakeShareable(new PlayerFSM(this));

	SetAtkRange(250);
	
	m_PawnInfo->SetGreen();
	
	m_PawnInfo->SetVisibility(true);

	m_SpawnPos = GetActorLocation();
}

void AMyPlayerPawn::SetGas(TSharedPtr<GAS> newGas)
{
	Super::SetGas(newGas);

	m_Gas.Pin()->m_OnHpChanged.AddUObject(this, &AMyPlayerPawn::OnHpChanged);

	m_Gas.Pin()->m_OnTookDamage.AddUObject(this, &AMyPlayerPawn::OnTookDamage);

	m_Gas.Pin()->SetDefaultStat(200,25);
}

void AMyPlayerPawn::OnHpChanged()
{
	m_PawnInfo->SetPawnInfo(this);
}

void AMyPlayerPawn::OnTookDamage(ACombatPawn* other, BigInt dmg, EDamagePopup pop)
{
	SetFocusedTarget(other);
	
	//PlayHittenSound(pop);
	//PlayHitFlash();
	//PlayTookHitMontage();
	//PlayHitEffect();
	//UMyGameInstance::Get->GetPlayerCon()->ShowInGameWorldText(dmg, this, pop);
}

void AMyPlayerPawn::OnDead()
{
	Super::OnDead();
	m_PawnInfo->SetVisibility(false);

	UMyGameInstance::Get->GetStageMode()->OnPlayerDead(this);
}

void AMyPlayerPawn::OnDeathAnimEnd()
{
	Super::OnDeathAnimEnd();

	UMyGameInstance::Get->GetStageMode()->OnPlayerDeadAnimEnd(this);
}



bool AMyPlayerPawn::IsInputMoving()
{
	return !m_Input.IsZero();
}

void AMyPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsAlive() || !m_Sensor)
	{
		return;
	}

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
	float t = TryAttack();

	if(t > 0)
	{
		m_AddVisual->SetAttacking(t);
	}
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
