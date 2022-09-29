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
	//
	TargetArmLength = 2200;

	m_RelativeSocketRotation = FQuat::Identity;

	CameraLagSpeed = 10.f;

	CameraLagMaxTimeStep = 1.f / 60.f;
}

void UCameraDissolve::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateDesiredArmLocation(DeltaTime);
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

	FVector ArmOrigin = GetComponentLocation() + m_TargetOffset;
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
