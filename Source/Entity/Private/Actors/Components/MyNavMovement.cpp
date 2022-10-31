#include "Actors/Components/MyNavMovement.h"

#include "Entity.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

UMyNavMovement::UMyNavMovement(const FObjectInitializer& obj): Super(obj)
{
	m_fSpeed = 1200.f;
	bUseAccelerationForPaths = true;
	bUseFixedBrakingDistanceForPaths = true;
	ResetMoveState();
	NavAgentProps.AgentHeight = 88;
	NavAgentProps.AgentRadius = 34;
	m_fMultiple = 1.0f;
	m_fRotateLerp = 8;
}

void UMyNavMovement::BeginPlay()
{
	Super::BeginPlay();
	m_Owner = GetOwner<AMyBasePawn>();
	NavAgentProps = m_Owner->GetNavAgentPropertiesRef();
}

void UMyNavMovement::SetMaxSpeed(float spd)
{
	m_fSpeed = spd;
}

void UMyNavMovement::SetImpact(FVector v)
{
	m_ImpactVector = v;
}

void UMyNavMovement::SetSpeedMultiple(float m)
{
	m_fMultiple = m;
}

void UMyNavMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	Velocity = FVector::ZeroVector;
	
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	ConsumeInputVector();

	m_Delta = GetDelta(DeltaTime, ControlAcceleration);

	FVector ActorLoc = GetActorLocation();

	FNavLocation NewLoc;

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if(Nav->ProjectPointToNavigation(ActorLoc, NewLoc))
	{
		m_Owner->SetActorFeetLocation(NewLoc);
	}

	if (IsNavBound(m_Delta) && !m_Delta.IsNearlyZero(1e-6f))
	{
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		const FQuat Rotation = UpdatedComponent->GetComponentQuat();

		FHitResult Hit(1.f);

		SafeMoveUpdatedComponent(m_Delta, Rotation, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, m_Delta);

			SlideAlongSurface(m_Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}
		Velocity = m_Delta;
	}

	m_ImpactVector = FVector::ZeroVector;

	UpdateComponentVelocity();

	TickRotate(DeltaTime);
}


void UMyNavMovement::SetActive(bool new_active, bool reset)
{
	Super::SetActive(new_active, reset);

	if (!new_active)
	{
		m_ImpactVector = FVector::ZeroVector;
	}
}

float UMyNavMovement::GetMaxSpeed() const
{
	return m_fSpeed;
}

bool UMyNavMovement::IsNavBound(FVector delta)
{
	FVector ActorLoc = GetActorLocation();

	FNavLocation Loc;

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	return  Nav->ProjectPointToNavigation(delta + ActorLoc, Loc);
}

FVector UMyNavMovement::GetDelta(float delta, const FVector& inputDelta)
{
	return m_Delta = (delta * m_fMultiple * inputDelta * GetMaxSpeed()) + m_ImpactVector;
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
		DesiredRotation.Yaw = UKismetMathLibrary::RInterpTo(CurrentRotation, DesiredRotation, deltaTime, m_fRotateLerp).Yaw;

		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, false);
	}
}

FRotator UMyNavMovement::ComputeOrientToMovementRotation(const FRotator& CurrentRotation) const
{
	if (m_Delta.IsNearlyZero(0.01f))
	//회전각이 0이여서 // 몬스터의 경우 추적 대상이 존재한다면 추적대상을 바라봐야함,이함수랑 별개로 만들어야할듯? ㄴㄴ 그냥 움직일때는 고개돌리는게 맞을듯
	{
		return CurrentRotation;
	}

	return m_Delta.GetSafeNormal().Rotation();
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
