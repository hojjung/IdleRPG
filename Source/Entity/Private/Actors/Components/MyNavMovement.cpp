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
	NavAgentProps = m_Owner->GetNavAgentPropertiesRef(); 
	MySnapToNav();
}

void UMyNavMovement::MySnapToNav()
{
	FVector ActorLoc = GetActorLocation();
	FNavLocation Loc;

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if(!Nav->ProjectPointToNavigation(ActorLoc,Loc))
	{
		Nav->GetRandomPointInNavigableRadius(ActorLoc,10,Loc);
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

	LimitWorldBounds();

	//bPositionCorrected = false;

	m_Delta = (Velocity * DeltaTime * m_fSpeedMultiple) + m_ImpactVector;

	if (!m_Delta.IsNearlyZero(1e-6f) && IsNavBound(m_Delta))
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

		const FVector NewLocation = UpdatedComponent->GetComponentLocation();
		
		Velocity = ((NewLocation - OldLocation) / DeltaTime);
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

bool UMyNavMovement::IsNavBound(FVector delta)
{
	FVector ActorLoc = GetActorLocation();
	
	FNavLocation Loc;

	UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	return Nav->ProjectPointToNavigation(delta + ActorLoc,Loc);
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

