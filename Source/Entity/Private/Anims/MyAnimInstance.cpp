#include "Anims/MyAnimInstance.h"

#include "DataAsset/EntityBaseAsset.h"

void FMyAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::InitializeObjects(InAnimInstance);
	
	m_MyAnim = Cast<UMyAnimInstance>(InAnimInstance);
}

void FMyAnimInstanceProxy::Update(float DeltaSeconds)
{
	m_MyAnim->UpdateFlag(DeltaSeconds);
}

void UMyAnimInstance::Init(const UUnitEntityAsset* asset)
{
	m_Idle = asset->m_Idle;
	
	m_Run = asset->m_Run;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!m_Owner)
	{
		return;
	}

	if(IsSlotPlaying())
	{
		return;
	}

	if(!m_bIsMoving)
	{
		m_Owner->GetSkMesh()->PlayAnimation(m_Idle,true);
	}
	else
	{
		m_Owner->GetSkMesh()->PlayAnimation(m_Run,true);
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
