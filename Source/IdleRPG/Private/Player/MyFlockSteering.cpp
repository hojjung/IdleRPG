#include "Player/MyFlockSteering.h"

#include "Manager/MyGameInstance.h"
#include "Monsters/MonsterPawn.h"

UMyFlockSteering::UMyFlockSteering(const FObjectInitializer& obj): Super(obj)
{
	m_fRadius = 400;
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

	return m_Delta = (delta * m_fMultiple * NewDelta * GetMaxSpeed()) + m_ImpactVector;	
}

FVector UMyFlockSteering::GetBoidDelta(FVector inputDelta)
{
	m_NearMobs.Reset();
	
	UMyGameInstance::Get->GetStageMode()->GetNearNpcs<AMonsterPawn>(m_Owner.Get(),m_NearMobs,m_fRadius);
	
	FVector DestDelta = inputDelta;
	
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
	
	FVector FinalDelta = (DestDelta * 1.35f) + (SepSum.GetSafeNormal(0.01f)); 
	
	return FinalDelta.GetSafeNormal();
}

void UMyFlockSteering::SetComponentTickEnabled(bool bEnabled)
{
	Super::SetComponentTickEnabled(bEnabled);

	Velocity = FVector::ZeroVector;
}
