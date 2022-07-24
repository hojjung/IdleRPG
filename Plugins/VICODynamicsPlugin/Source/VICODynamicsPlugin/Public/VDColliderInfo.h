// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "PhysicsPublic.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "Components/PrimitiveComponent.h"
#include "PhysXPublic.h"

// Preps and holds Collider info for Explicit Collider checks.
struct VICODYNAMICSPLUGIN_API FExplicitColliderInfo
{
	FExplicitColliderInfo(UPrimitiveComponent* InCollider, bool IgnoreConvexMeshShapes, bool SkipInit = false) : Collider(InCollider), SkipChecks(false)
	{
		if (!SkipInit)
		{
			const auto BI = Collider->GetBodyInstance();
			if (BI)
			{
				PxActor = FPhysicsInterface::GetPxRigidActor_AssumesLocked(BI->GetPhysicsActorHandle());
				
				TArray<FPhysicsShapeHandle> Shapes;
				BI->GetAllShapes_AssumesLocked(Shapes);

				for (int32 i = 0; i < Shapes.Num(); i++)
				{
					if (Shapes[i].IsValid())
					{
						auto Shape = Shapes[i].Shape;
						if (IgnoreConvexMeshShapes && Shape->getGeometryType() == PxGeometryType::eCONVEXMESH)
							continue;

						PxShapes.Add(Shape);
					}
				}
			}
		}
	}

	FORCEINLINE_DEBUGGABLE bool IsValid() const
	{
		return Collider /*&& !Collider->IsPendingKill()*/ && PxShapes.Num();
	}

	bool operator==(const FExplicitColliderInfo& other) const
	{
		return Collider == other.Collider;
	}

	UPrimitiveComponent* Collider;
	PxRigidActor* PxActor;
	TArray<PxShape*> PxShapes;
	bool SkipChecks;
};