#include "Actors/Components/MyNavMovement.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

UMyNavMovement::UMyNavMovement(const FObjectInitializer& obj): Super(obj)
{
	MaxSpeed = 1200.f;
	Acceleration = 4000.f;
	Deceleration = 8000.f;
	bUseAccelerationForPaths = true;
	bUseFixedBrakingDistanceForPaths = true;
	TurningBoost = 8.0f;
	bPositionCorrected = false;
	m_fSpeedMultiple = 1;
	ResetMoveState();
	NavAgentProps.AgentHeight = 88;
	NavAgentProps.AgentRadius = 34;
}

void UMyNavMovement::BeginPlay()
{
	Super::BeginPlay();
	m_Owner = GetOwner<AMyBasePawn>();
	MySnapToNav();
}

void UMyNavMovement::MySnapToNav()
{
	FVector ActorLoc = GetActorLocation();
	FNavLocation Loc;

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if(!Nav->ProjectPointToNavigation(ActorLoc,Loc))
	{
		Nav->GetRandomPointInNavigableRadius(ActorLoc,1000,Loc);
	}
	
	m_Owner->SetActorFeetLocation(Loc.Location);
}

void UMyNavMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	UMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	ApplyControlInputToVelocity(DeltaTime);

	//LimitWorldBounds();

	//bPositionCorrected = false;

	FVector Delta = (Velocity * DeltaTime * m_fSpeedMultiple) + m_ImpactVector;

	if (!Delta.IsNearlyZero(1e-6f))
	{
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		const FQuat Rotation = UpdatedComponent->GetComponentQuat();

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, false, Hit);

		// if (Hit.IsValidBlockingHit())
		// {
		// 	HandleImpact(Hit, DeltaTime, Delta);
		// 	
		// 	SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		// }

		//if (!bPositionCorrected)
		{
			const FVector NewLocation = UpdatedComponent->GetComponentLocation();
			Velocity = ((NewLocation - OldLocation) / DeltaTime);
		}

		MySnapToNav();
	}

	m_ImpactVector = FVector::ZeroVector;
	
	UpdateComponentVelocity();

	TickRotate(DeltaTime);
}

void UMyNavMovement::SetImpact(FVector v)
{
	m_ImpactVector = v;
}

void UMyNavMovement::SetActive(bool new_active, bool reset)
{
	Super::SetActive(new_active, reset);

	if(!new_active)
	{
		m_ImpactVector = FVector::ZeroVector;
	}
}

void UMyNavMovement::TickRotate(float deltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	FRotator DesiredRotation = CurrentRotation;

	DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation);
	DesiredRotation.Pitch = 0.f;
	DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
	DesiredRotation.Roll = 0.f;

	if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, 1e-3f))
	{
		DesiredRotation.Yaw = UKismetMathLibrary::RInterpTo(CurrentRotation, DesiredRotation, deltaTime, 8).Yaw;

		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, false);
	}
}

FRotator UMyNavMovement::ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const
{
	if (Velocity.IsNearlyZero(0.01f))
		//회전각이 0이여서 // 몬스터의 경우 추적 대상이 존재한다면 추적대상을 바라봐야함,이함수랑 별개로 만들어야할듯? ㄴㄴ 그냥 움직일때는 고개돌리는게 맞을듯
	{
		return CurrentRotation;
	}

	return Velocity.GetSafeNormal().Rotation();
}

void UMyNavMovement::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	IPathFollowingAgentInterface* PFAgent = GetPathFollowingAgent();
	if (PFAgent)
	{
		// Also notify path following!
		PFAgent->OnMoveBlockedBy(Hit);
	}

	APawn* OtherPawn = Cast<APawn>(Hit.GetActor());
	
	if (OtherPawn)
	{
		NotifyBumpedPawn(OtherPawn);
	}
}

bool UMyNavMovement::CanStepUp(const FHitResult& Hit) const
{
	if (!Hit.IsValidBlockingHit())
		return false;
	const UPrimitiveComponent* HitComponent = Hit.Component.Get();
	if (!HitComponent)
		return true;
	if (!HitComponent->CanCharacterStepUp(m_Owner.Get()))
		return false;
	const AActor* HitActor = Hit.GetActor();
	if (!HitActor)
		return true;
	if (!HitActor->CanBeBaseForCharacter(m_Owner.Get()))
		return false;
	return true;
}

