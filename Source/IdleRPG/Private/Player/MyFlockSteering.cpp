#include "Player/MyFlockSteering.h"

#include "Manager/MyGameInstance.h"
#include "Monsters/MonsterPawn.h"

UMyFlockSteering::UMyFlockSteering(const FObjectInitializer& obj): Super(obj)
{
	m_fRadius = 400;
	m_fRotateLerp = 4;
}

void UMyFlockSteering::BeginPlay()
{
	Super::BeginPlay();
	m_NearMobs.Reserve(20);
}

FVector UMyFlockSteering::GetDelta(float delta, const FVector& inputDelta)
{
	FVector NewDelta;
	
	if(m_Owner->UseBoidMove())
	{
		NewDelta = GetBoidDelta(inputDelta);
	}
	else
	{
		NewDelta = inputDelta;	
	}

	FVector Lerp = UKismetMathLibrary::VLerp(inputDelta, NewDelta, 0.5f);
	
	return Super::GetDelta(delta, Lerp);	
}

FVector UMyFlockSteering::GetBoidDelta(FVector inputDelta)
{
	m_NearMobs.Reset();
	
	UMyGameInstance::Get->GetStageMode()->GetNearNpcs<AMonsterPawn>(m_Owner.Get(),m_NearMobs,m_fRadius);
	
	FVector SepSum = FVector::ZeroVector;
	
	FVector OwnerLoc = m_Owner->GetActorLocation();
	
	OwnerLoc.Z = 0.f;

	if (m_NearMobs.Num() > 0)
	{
		int Count = 1;
		
		for (ACombatPawn* OtherActor : m_NearMobs)
		{
			if(!OtherActor->IsAlive())
			{
				continue;
			}
			FVector OtherLoc = OtherActor->GetActorLocation();
			
			OtherLoc.Z = 0.f;
			
			SepSum += (OwnerLoc - OtherLoc);
		}
		SepSum /= Count;
	}

	FVector Result = (SepSum.GetSafeNormal(0.01f)) + (inputDelta);
	
	return Result.GetSafeNormal(0.01f);
}

void UMyFlockSteering::SetComponentTickEnabled(bool bEnabled)
{
	Super::SetComponentTickEnabled(bEnabled);

	Velocity = FVector::ZeroVector;
}
