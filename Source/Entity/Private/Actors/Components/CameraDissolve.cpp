#include "Actors/Components/CameraDissolve.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "UObject/ConstructorHelpers.h"


UCameraDissolve::UCameraDissolve()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	bTickInEditor = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> FoundCollection(
		TEXT("MaterialParameterCollection'/Game/03_VisualEffect/PC_WallDissolveAndFog.PC_WallDissolveAndFog'"));

	m_MatParamAsset = FoundCollection.Object;

	m_fDissloveAmount = 0.f;
	m_fDissolveMaxAmount = 0.4f; //0.35f;
	m_fDissolveHoleRadius = 0.f;
	m_fDissolveHoleMaxRadius = 90;
	m_fDissolvingTime = 0.5f;
	m_fTimer = 0.f;
	m_bWasBlocked = false;
	//
	TargetArmLength = 2200;

	m_RelativeSocketRotation = FQuat::Identity;

	CameraLagSpeed = 10.f;

	CameraLagMaxTimeStep = 1.f / 60.f;
}

void UCameraDissolve::StartDissolve()
{
	m_fTimer = m_fDissolvingTime;
}

void UCameraDissolve::EndDissolve()
{
	m_fTimer = 0.35f; //꺼지라고 해놓은듯
}

void UCameraDissolve::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateDesiredArmLocation(DeltaTime);
	ExecuteDissolve(DeltaTime);
	SetValueParameter();
}


void UCameraDissolve::ExecuteDissolve(float DeltaTime)
{
	if (m_fTimer > 0.f)
	{
		m_fTimer -= DeltaTime;

		if (m_bWasBlocked)
		{
			m_fDissloveAmount = FMath::FInterpTo(m_fDissloveAmount, m_fDissolveMaxAmount, DeltaTime, 5.f);

			m_fDissolveHoleRadius = FMath::FInterpTo(m_fDissolveHoleRadius, m_fDissolveHoleMaxRadius, DeltaTime, 5.f);
		}
		else
		{
			m_fDissloveAmount = FMath::FInterpTo(m_fDissloveAmount, 0.f, DeltaTime, 5);

			m_fDissolveHoleRadius = FMath::FInterpTo(m_fDissolveHoleRadius, 0.f, DeltaTime, 5);
		}
	}
}


void UCameraDissolve::SetValueParameter()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetOwner(), m_MatParamAsset, "Amount", m_fDissloveAmount);
	UKismetMaterialLibrary::SetScalarParameterValue(GetOwner(), m_MatParamAsset, "Radius", m_fDissolveHoleRadius);
	//
	UKismetMaterialLibrary::SetVectorParameterValue(GetOwner(), m_MatParamAsset, "Position1", m_PreviousDesiredLoc);
	UKismetMaterialLibrary::SetVectorParameterValue(GetOwner(), m_MatParamAsset, "Position2", m_CamPos);
}

FTransform UCameraDissolve::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	FTransform RelativeTransform(m_RelativeSocketRotation, m_RelativeSocketLocation);

	switch (TransformSpace)
	{
	case RTS_World:
		{
			return RelativeTransform * GetComponentTransform();
		}
	case RTS_Actor:
		{
			if (const AActor* Actor = GetOwner())
			{
				FTransform SocketTransform = RelativeTransform * GetComponentTransform();
				return SocketTransform.GetRelativeTransform(Actor->GetTransform());
			}
			break;
		}
	case RTS_Component:
		{
			return RelativeTransform;
		}
	}
	return RelativeTransform;
}

void UCameraDissolve::UpdateDesiredArmLocation(float DeltaTime)
{
	FRotator DesiredRot = GetRelativeRotation();

	FVector ArmOrigin = GetComponentLocation();
	// We lag the target, not the actual camera position, so rotating the camera around does not have lag
	FVector DesiredLoc = ArmOrigin;
	if (DeltaTime > CameraLagMaxTimeStep && CameraLagSpeed > 0.f)
	{
		const FVector ArmMovementStep = (DesiredLoc - m_PreviousDesiredLoc) * (1.f / DeltaTime);
		FVector LerpTarget = m_PreviousDesiredLoc;

		float RemainingTime = DeltaTime;
		while (RemainingTime > KINDA_SMALL_NUMBER)
		{
			const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
			LerpTarget += ArmMovementStep * LerpAmount;
			RemainingTime -= LerpAmount;

			DesiredLoc = FMath::VInterpTo(m_PreviousDesiredLoc, LerpTarget, LerpAmount, CameraLagSpeed);
			m_PreviousDesiredLoc = DesiredLoc;
		}
	}

	m_PreviousArmOrigin = ArmOrigin;

	m_PreviousDesiredLoc = DesiredLoc;
	//
	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(m_SocketOffset);

	// Do a sweep to ensure we are not penetrating the world
	FVector ResultLoc;
	if (TargetArmLength != 0.0f)
	{
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());
		FHitResult Result;
		GetWorld()->SweepSingleByChannel(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ECC_Camera,
		                                 FCollisionShape::MakeSphere(m_fDissolveHoleMaxRadius), QueryParams);
		//

		ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);

		if (Result.bBlockingHit)
		{
			if (!m_bWasBlocked)
			{
				StartDissolve();
			}
		}
		else
		{
			if (m_bWasBlocked)
			{
				EndDissolve();
			}
		}
		m_bWasBlocked = Result.bBlockingHit;
	}


	// Form a transform for new world transform for camera
	m_CamPos = DesiredLoc;
	FTransform WorldCamTM(DesiredRot, DesiredLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());

	// Update socket location/rotation
	m_RelativeSocketLocation = RelCamTM.GetLocation();
	m_RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();
}

FVector UCameraDissolve::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation,
                                        bool bHitSomething, float DeltaTime)
{
	return bHitSomething ? TraceHitLocation : DesiredArmLocation;
}
