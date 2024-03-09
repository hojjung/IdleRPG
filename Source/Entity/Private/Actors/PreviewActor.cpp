#include "Actors/PreviewActor.h"
#include "Entity.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TextureRenderTarget2D.h"

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
	//m_MeshBody->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;//최적화
	m_MeshBody->bAffectDynamicIndirectLighting = true;
	m_MeshBody->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_MeshBody->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_MeshBody->bEnableUpdateRateOptimizations = true;
	m_MeshBody->bComponentUseFixedSkelBounds = true;
	//
	m_bTouched = false;

}

void APreviewActor::BeginPlay()
{
	Super::BeginPlay();

	m_InitVisualRot = m_MeshBody->GetComponentRotation();
}

void APreviewActor::SetEntity(const UUnitAsset* asset)
{
	m_MeshBody->SetAnimClass(nullptr);
	m_MeshBody->SetSkeletalMesh(asset->m_BodyMesh.Get());

	if (asset->m_IdleAnim.Get())
	{
		m_MeshBody->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		m_MeshBody->PlayAnimation(asset->m_IdleAnim.Get(), true);
	}
	else
	{
		m_MeshBody->SetAnimClass(asset->m_ClassAnim);
		m_MeshBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	m_MeshBody->AddRelativeRotation(FRotator(0, asset->m_RotYawOffset, 0));

	m_MeshBody->SetRelativeScale3D(FVector(asset->m_fScale));

	if (m_Visual != nullptr)
	{
		m_Visual->Hide();
	}

	m_Visual = NewObject<UAvatarAddtionalVisuals>(this);
	m_Visual->Init(m_MeshBody);

	int Index = 0;
	for (const auto& Attach : asset->m_AryAttachments)
	{
		m_Visual->SpawnAttachment(asset->m_ArySocketAttachments[Index++], Attach.Get());
	}
	m_Visual->TickWind(false, 0);
}

void APreviewActor::SetMeshScale(float s)
{
	m_MeshBody->SetRelativeScale3D(FVector(s));
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

float APreviewActor::PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate, FName StartSectionName, float sectionDur)
{
	UAnimInstance* AnimInstance = m_MeshBody->GetAnimInstance();

	if (anim_montage && AnimInstance)
	{
		float AssetDur = AnimInstance->Montage_Play(anim_montage, InPlayRate);

		if (AssetDur > 0.f)
		{
			FName SectioNName;

			if (StartSectionName != NAME_None) //섹션지정시
			{
				SectioNName = StartSectionName;
			}
			else
			{
				SectioNName = anim_montage->GetSectionName(0);
			}

			if (sectionDur < 0)
			{
				sectionDur = GetSectionLength(SectioNName, anim_montage);
			}
			AnimInstance->Montage_JumpToSection(SectioNName, anim_montage);

			sectionDur = (sectionDur / (InPlayRate * anim_montage->RateScale)); //가속된만큼 빠르게

			//ClearStopMoveDelegate();
			//m_Movement->SetActive(false);
			//GetWorldTimerManager().SetTimer(m_MoveStopTimer, this, &APreviewActor::ActiveMovement, sectionDur, false);

			return sectionDur;
		}
	}
	return 0.f;
}


float APreviewActor::GetSectionLength(FName sectionName, const UAnimMontage* anim_montage)
{
	UAnimInstance* AnimInstance = m_MeshBody->GetAnimInstance();

	AnimInstance->Montage_JumpToSection(sectionName, anim_montage);

	int Index = anim_montage->GetSectionIndex(sectionName);

	return anim_montage->GetSectionLength(Index);
}

float APreviewActor::PlayAnimMontageSetDuration(UAnimMontage* anim_montage, float setDur, FName StartSectionName)
{
	UAnimInstance* AnimInstance = m_MeshBody->GetAnimInstance();

	if (anim_montage && AnimInstance)
	{
		float AssetDur = setDur;

		if (!StartSectionName.IsNone())
		{
			AssetDur = GetSectionLength(StartSectionName, anim_montage);
					}

		float NewRate = AssetDur / setDur;

		return PlayAnimMontage(anim_montage, NewRate, StartSectionName, AssetDur);
	}
	return 0.f;
}

void APreviewActor::StopAnimMontage()
{
	UAnimInstance* AnimInstance = m_MeshBody->GetAnimInstance();

	UAnimMontage* MontageToStop = GetCurrentMontage();

	bool bShouldStopMontage = AnimInstance && MontageToStop && !AnimInstance->Montage_GetIsStopped(MontageToStop);

	if (bShouldStopMontage)
	{
		AnimInstance->Montage_Stop(MontageToStop->BlendOut.GetBlendTime(), MontageToStop);
	}
}

UAnimMontage* APreviewActor::GetCurrentMontage()
{
	UAnimInstance* AnimInstance = m_MeshBody->GetAnimInstance();

	if (AnimInstance)
	{
		return AnimInstance->GetCurrentActiveMontage();
	}

	return nullptr;
}

bool APreviewActor::PlayMontageIndexDur(UAnimMontage* anim_montage, int index, float dur)
{
	const TArray<FCompositeSection>& AnimAry = anim_montage->CompositeSections;

	if (index < 0 || index >= AnimAry.Num())
	{
		return false;
	}

	float Length = PlayAnimMontageSetDuration(anim_montage, dur, AnimAry[index].SectionName);

	return Length != 0.0f;
}