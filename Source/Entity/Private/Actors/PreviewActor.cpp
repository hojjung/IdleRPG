#include "Actors/PreviewActor.h"
#include "Entity.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetMathLibrary.h"

APreviewActor::APreviewActor()
{
	PrimaryActorTick.bCanEverTick = true;
	//init skMesh
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	//
	m_MeshBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	m_MeshBody->SetupAttachment(RootComponent);
	m_MeshBody->SetCollisionProfileName(TEXT("NoCollision"));
	m_MeshBody->SetGenerateOverlapEvents(false);
	m_MeshBody->SetCanEverAffectNavigation(false);
	m_MeshBody->SetRelativeLocation(FVector(0, 0, -88));
	m_MeshBody->SetRelativeRotation(FRotator(0, -90.f, 0.f));
	m_MeshBody->SetCollisionProfileName(TEXT("CharacterMesh"));
	m_MeshBody->bReceivesDecals = false;
	m_MeshBody->bOwnerNoSee = false;
	//
	m_MeshBody->bCastDynamicShadow = false; //chanage for mobile
	m_MeshBody->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;//최적화
	m_MeshBody->bAffectDynamicIndirectLighting = true;
	m_MeshBody->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshBody->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_MeshBody->bEnableUpdateRateOptimizations = true;
	m_MeshBody->bComponentUseFixedSkelBounds = true;
	//
	m_Spring = CreateDefaultSubobject<USpringArmComponent>("Spring");
	m_Spring->SetupAttachment(RootComponent);
	m_Spring->SetRelativeLocation(FVector(0,0,30));
	m_Spring->SetRelativeRotation(FRotator(-5, 200.f, 0));
	m_Spring->TargetArmLength = 350;
	m_Spring->bDoCollisionTest = 0;

	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");
	m_Capture->SetupAttachment(m_Spring);
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> FoundTexture(TEXT("TextureRenderTarget2D'/Game/03_VisualEffect/T_PlayerPreviewAlpha.T_PlayerPreviewAlpha'"));
	m_Capture->TextureTarget = FoundTexture.Object;
	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	m_Capture->SetTickableWhenPaused(true);
	m_Capture->ProjectionType = ECameraProjectionMode::Perspective;
	m_Capture->FOVAngle = 45;
	m_Capture->PostProcessBlendWeight = 0.f;
	//
	m_bTouched = false;

	m_MeshBody->LightingChannels.bChannel0 = 0;
	m_MeshBody->LightingChannels.bChannel1 = 1;
}

void APreviewActor::BeginPlay()
{
	Super::BeginPlay();

	m_Capture->ShowOnlyActors.Add(this);

	m_InitVisualRot = m_MeshBody->GetComponentRotation();

	HideMeshWithTick();
}

void APreviewActor::SetEntity(const UUnitAsset* asset)
{
	m_MeshBody->SetAnimClass(nullptr);
	m_MeshBody->SetSkeletalMesh(asset->m_BodyMesh.Get());
	m_MeshBody->SetAnimClass(asset->m_ClassAnim);
	m_MeshBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	m_MeshBody->AddRelativeRotation(FRotator(0,asset->m_RotYawOffset,0));
}

void APreviewActor::SetMeshScale(float s)
{
	m_MeshBody->SetRelativeScale3D(FVector(s));
}

void APreviewActor::ShowMeshWithTick()
{
	m_MeshBody->SetVisibility(true);

	m_MeshBody->SetComponentTickEnabled(true);

	m_Capture->SetComponentTickEnabled(true);

	PRINTF("ShowMeshWithTick");
}

void APreviewActor::HideMeshWithTick()
{
	m_MeshBody->SetVisibility(false);

	m_MeshBody->SetComponentTickEnabled(false);

	m_Capture->SetComponentTickEnabled(false);

	PRINTF("HideMeshWithTick");
}

void APreviewActor::SetIsTouched(bool b)
{
	m_bTouched = b;
}

void APreviewActor::RotatePawn(float delta_x)
{
	delta_x = -1.f * delta_x;
	FRotator Rot(0.f);
	Rot.Yaw = delta_x;
	m_MeshBody->AddLocalRotation(Rot);
}

void APreviewActor::Tick(float delta)
{
	Super::Tick(delta);

	CalculateVisualActorRot(delta);
}

void APreviewActor::CalculateVisualActorRot(float delta)
{
	if (m_bTouched)
	{
		return;
	}

	FRotator NewRot = m_MeshBody->GetComponentRotation();

	NewRot.Yaw = UKismetMathLibrary::RInterpTo(NewRot, m_InitVisualRot, delta, 5.5f).Yaw;

	m_MeshBody->SetWorldRotation(NewRot);
}

