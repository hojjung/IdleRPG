#include "Anims/MyAnimInstance.h"

#include "Entity.h"
#include "Animation/AnimNode_StateMachine.h"
#include "DataAsset/EntityBaseAsset.h"

void FMyAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::InitializeObjects(InAnimInstance);
	
	m_MyAnim = Cast<UMyAnimInstance>(InAnimInstance);
}

void FMyAnimInstanceProxy::Update(float DeltaSeconds)
{
	Super::Update(DeltaSeconds);
	//m_MyAnim->UpdateFlag(DeltaSeconds);
}

void FMyAnimInstanceProxy::UpdateAnimationNode(const FAnimationUpdateContext& InContext)
{
	FAnimInstanceProxy::UpdateAnimationNode(InContext);
}
void UMyAnimInstance::NativeInitializeAnimation()
{
	AddNativeStateEntryBinding(TEXT("MyFSM"), TEXT("Idle"), FOnGraphStateChanged::CreateUObject(this, &UMyAnimInstance::OnIdle));
	AddNativeStateEntryBinding(TEXT("MyFSM"), TEXT("Run"), FOnGraphStateChanged::CreateUObject(this, &UMyAnimInstance::OnRun));
	AddNativeTransitionBinding(TEXT("MyFSM"),TEXT("Idle"),TEXT("Run"),FCanTakeTransition::CreateUObject(this, &UMyAnimInstance::IdleToRun));
	AddNativeTransitionBinding(TEXT("MyFSM"),TEXT("Run"),TEXT("Idle"),FCanTakeTransition::CreateUObject(this, &UMyAnimInstance::RunToIdle));
	Super::NativeInitializeAnimation();
}
void UMyAnimInstance::Init(const UUnitEntityAsset* asset, AMyBasePawn* pawn)
{
	
}

bool UMyAnimInstance::RunToIdle()
{
	return !m_Owner->IsMoving();
}

bool UMyAnimInstance::IdleToRun()
{
	return m_Owner->IsMoving();
}

void UMyAnimInstance::OnIdle(const FAnimNode_StateMachine& mc, int32 prev, int32 next)
{
	PRINTF("OnIdle");
}

void UMyAnimInstance::OnRun(const FAnimNode_StateMachine& mc, int32 prev, int32 next)
{
	PRINTF("OnRun");
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!m_Owner)
	{
		return;
	}
	
}

bool UMyAnimInstance::IsSlotPlaying()
{
	if(!GetActiveMontageInstance())
	{
		return false;
	}
	return GetActiveMontageInstance()->IsActive();
}



float UMyAnimInstance::PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate, FName StartSectionName)
{
	float Duration = Montage_Play(anim_montage, InPlayRate);

	if (Duration > 0.f)
	{
		FName SectioNName;

		if (StartSectionName != NAME_None)
		{
			SectioNName = StartSectionName;
		}
		else
		{
			SectioNName = anim_montage->GetSectionName(0);
		}

		Montage_JumpToSection(SectioNName, anim_montage);

		int Index = anim_montage->GetSectionIndex(SectioNName);

		Duration = anim_montage->GetSectionLength(Index);

		Duration = (Duration / (InPlayRate * anim_montage->RateScale));

		return Duration;
	}
	return 0.f;
}

void UMyAnimInstance::StopAnimMontage()
{
	if (!GetActiveMontageInstance())
	{
		return;
	}
	UAnimMontage* MontageToStop = GetActiveMontageInstance()->Montage;
	
	bool bShouldStopMontage = MontageToStop && !Montage_GetIsStopped(MontageToStop);

	if (bShouldStopMontage)
	{
		Montage_Stop(MontageToStop->BlendOut.GetBlendTime(), MontageToStop);
	}
}

void UMyAnimInstance::UpdateFlag(float deltaTime)
{
	//#if WITH_EDITOR
	if(!m_Owner)
	{
		return;
	}
	//#endif
	m_bIsMoving=m_Owner->IsMoving();
}
