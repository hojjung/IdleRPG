#include "Player/MyFlockSteering.h"

#include "MyGameInstance.h"
#include "Monsters/MonsterPawn.h"

void UMyFlockSteering::BeginPlay()
{
	Super::BeginPlay();
	m_NearMobs.Reserve(20);
}

FVector UMyFlockSteering::GetFlockDir()
{
	m_NearMobs.Reset();
	
	FVector TargetLoc = GetOwner()->GetActorLocation();
	TargetLoc.Z = 0.f;

	UMyGameInstance::Get->m_SpawnManager->GetNearNpcs(m_Owner, m_NearMobs, 500);
	
	FVector Sum = FVector::ZeroVector;

	int Count = 0;

	for (auto& OtherActor : m_NearMobs)
	{
		if (!OtherActor->IsAlive())
		{
			//continue;
		}
		FVector OtherLoc = OtherActor->GetActorLocation();
		OtherLoc.Z = 0.f;

		FVector Diff = (TargetLoc - OtherLoc).GetSafeNormal();

		Sum += Diff;

		Count++;
	}

	if(Count > 0)
	{
		Sum /= Count;
		
		return Sum;
	}

	return FVector::ZeroVector;
}

void UMyFlockSteering::ApplyControlInputToVelocity(float DeltaTime)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
	const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (Velocity.SizeSquared() > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			const float TimeScale = FMath::Clamp(DeltaTime * TurningBoost, 0.f, 1.f);
			Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * TimeScale;
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if (Velocity.SizeSquared() > 0.f)
		{
			const FVector OldVelocity = Velocity;
			const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(Deceleration) * DeltaTime, 0.f);
			Velocity = Velocity.GetSafeNormal() * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if (bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
			}
		}
	}
	
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
	
	FVector FlockDir = GetFlockDir();

	FVector NewDelta;

	if(!FlockDir.IsNearlyZero())
	{
		NewDelta = (FlockDir + ControlAcceleration) / 2.f;
	}
	else
	{
		NewDelta = ControlAcceleration;	
	}
	
	Velocity += NewDelta * FMath::Abs(Acceleration) * DeltaTime;
	//둘의 차이가 너무크니까 지터링되는것
	
	Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);

	ConsumeInputVector();
}

void UMyFlockSteering::NotifyBumpedPawn(APawn* BumpedPawn)
{
	
}
