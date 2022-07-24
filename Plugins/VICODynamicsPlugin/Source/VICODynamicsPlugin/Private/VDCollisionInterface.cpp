// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDCollisionInterface.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "BaseVDComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "VDColliderInfo.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "DrawDebugHelpers.h"

FORCEINLINE physx::PxVec3 VDVectorToPVector(const vicoDynamics::FVector3Data& VDVec)
{
	return physx::PxVec3(VDVec.X, VDVec.Y, VDVec.Z);
}
FORCEINLINE vicoDynamics::FVector3Data PVectorToVDVector(const physx::PxVec3& PVec)
{
	return vicoDynamics::FVector3Data(PVec.x, PVec.y, PVec.z);
}

// inline uint32 GetTypeHash(const FOverlapCacheKey& A)
// {
// 	return GetTypeHash(A.Get<1>());
// }
// 
// inline uint32 GetTypeHash(const FSweepCacheKey& A)
// {
// 	return GetTypeHash(A.Get<3>());
// }
// 
// inline bool operator==(const FOverlapCacheKey& A, const FOverlapCacheKey& B)
// {
// 	return A.Get<1>() == B.Get<1>();
// }
// 
// inline bool operator==(const FSweepCacheKey& A, const FSweepCacheKey& B)
// {
// 	return A.Get<3>() == B.Get<3>();
// }

// inline uint32 GetTypeHash(const FOverlapCacheKey A)
// {
// 	return GetTypeHash((const void*)A);
// }
// 
// inline uint32 GetTypeHash(const FSweepCacheKey A)
// {
// 	return GetTypeHash((const void*)A);
// }

// inline bool operator==(const FOverlapCacheKey A, const FOverlapCacheKey B)
// {
// 	return A == B;
// }
// 
// inline bool operator==(const FSweepCacheKey A, const FSweepCacheKey B)
// {
// 	return A == B;
// }

FVDCollisionQueryInterface::FVDCollisionQueryInterface(UVDSimulation* InSimulation, bool CachePerFrameResults /*= true*/, bool CacheOnlyCollisionsWithStaticRigidBodies /*= false*/, bool CacheNonHitsAsWell /*= true*/)
	: Simulation(InSimulation), bCachePerFrameResults(CachePerFrameResults), bCacheOnlyCollisionsWithStaticRigidBodies(CacheOnlyCollisionsWithStaticRigidBodies), bCacheNonHitsAlso(CacheNonHitsAsWell)
{
	CollisionShape = FCollisionShape::MakeSphere(5.f);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	DrawCollisionDebug = false;
#endif
}

bool FVDCollisionQueryInterface::OverlapSphere(float Radius, const vicoDynamics::FVector3Data& Location, vicoDynamics::FQueryResults* Results, const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject)
{
	SCOPE_CYCLE_COUNTER(STAT_CIOverlap);

	bool Hit = false;
	
	const auto VDSimObj = static_cast<UVDSimulatedObject*>(SimObject->UserData);
	const auto VDComp = Cast<UBaseVDComponent>(VDSimObj->GetOuter());
	
	if (VDComp && Simulation->ExplicitRigidBodyCollisionEnabled && VDComp->UseExplicitRigidBodyCollision)
	{
		Hit = OverlapWithExplicitRigidBodies(VDComp, VDSimObj, Radius, Location, Results, Particle, SimObject);
	}
	else
	{
		const auto ULocation = VDVectorToUVector(Location);
		FOverlapCacheKey CacheKey = Particle;
		FOverlapCacheEntry* ValuePtr = nullptr;
		if (bCachePerFrameResults)
		{
#if VICODYNAMICS_USE_PARALLEL
			vicoDynamics::threading::FScopeReaderLock lock(OverlapCacheCritSec);
//			if (OverlapCache.Contains(CacheKey))
#endif // VICODYNAMICS_USE_PARALLEL
			ValuePtr = OverlapCache.Find(CacheKey);
		}

		// Check the SweepCache first
		if (ValuePtr)
		{
			SCOPE_CYCLE_COUNTER(STAT_CIOverlapCacheHit);

			//const auto& Value = *ValuePtr;

			const auto Actor = ValuePtr->Actor;
			const auto Shape = ValuePtr->Shape;

			if (!Actor || !Shape) return false; // Early out if no collision was made this frame
					
			Hit = PxGeometryQuery::overlap(PxSphereGeometry(Radius), PxTransform(U2PVector(ULocation)), Shape->getGeometry().any(), Actor->getGlobalPose());
			if (Hit)
			{
				Results->AddResult(Shape, Actor);
				if (VDComp->bNotifyOnVDCollision)
				{					
					UPrimitiveComponent* OtherComp = FPhysxUserData::Get<FBodyInstance>(Actor->userData)->OwnerComponent.Get();
					const auto HitNormal = (OtherComp->GetComponentLocation() - ULocation).GetSafeNormal();
					VDComp->AddPendingCollisionNotification(VDParticleToUVDParticle(Particle), OtherComp, ULocation + HitNormal * Radius, HitNormal);
				}
			}
		}
		else
		{
			const auto World = Simulation->RunOnWorkerThread ? Simulation->SimulationThreadWorker->GetWorldRef() : Simulation->CurWorld;

			if (World)
			{
				const auto& Settings = VDSimObj->GetCollisionSettings();
				const auto VDSimParticle = VDParticleToUVDParticle(Particle);
				if (!VDSimParticle) return false;

				CollisionShape.SetSphere(Radius);
				TArray<FOverlapResult> Overlaps;
				const auto Overlapped = World->OverlapMultiByChannel(Overlaps, *reinterpret_cast<const FVector*>(&Location), FQuat::Identity, Settings.ObjectType, CollisionShape, ColParams, FCollisionResponseParams(Settings.CollisionResponse.GetResponseContainer()));
				if (Overlapped)
				{
					for (const auto& Overlap : Overlaps)
					{
						auto OverlappedComponent = Overlap.GetComponent();
						if (Overlap.bBlockingHit && OverlappedComponent)
						{
							if (!Particle->GetIsFree() && VDSimParticle->IsAttached && OverlappedComponent == VDSimParticle->Attachment.ComponentRef.GetComponent(VDComp->GetOwner()))
								continue;

							const auto skelMesh = Cast<USkeletalMeshComponent>(OverlappedComponent);
							const auto BodyInstance = skelMesh && skelMesh->Bodies.IsValidIndex(Overlap.ItemIndex) ? skelMesh->Bodies[Overlap.ItemIndex] : OverlappedComponent->GetBodyInstance();
							const auto Actor = FPhysicsInterface::GetPxRigidActor_AssumesLocked(BodyInstance->GetActorReferenceWithWelding());

							const auto shapesCount = Actor->getNbShapes();
							if (shapesCount <= 0)
								return false;
							physx::PxShape* Shape = nullptr;
							Actor->getShapes(&Shape, 1);
							Results->AddResult(Shape, Actor);
							if (VDComp->bNotifyOnVDCollision)
							{
								UPrimitiveComponent* OtherComp = FPhysxUserData::Get<FBodyInstance>(Actor->userData)->OwnerComponent.Get();
								const auto HitNormal = (OtherComp->GetComponentLocation() - ULocation).GetSafeNormal();
								VDComp->AddPendingCollisionNotification(VDSimParticle, OtherComp, ULocation + HitNormal * Radius, HitNormal);
							}
							Hit = true;

							if (bCachePerFrameResults)
							{
								if (!bCacheOnlyCollisionsWithStaticRigidBodies || (bCacheOnlyCollisionsWithStaticRigidBodies && Actor->is<PxRigidStatic>()))
								{
#if VICODYNAMICS_USE_PARALLEL
									vicoDynamics::threading::FScopeWriterLock lock(OverlapCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

									// Cache the Hit result
									FOverlapCacheEntry CacheValue = { Actor, Shape };
									OverlapCache.Add(MoveTemp(CacheKey), MoveTemp(CacheValue));
								}
							}
						}
					}
				}
				else if (bCachePerFrameResults && bCacheNonHitsAlso)
				{
#if VICODYNAMICS_USE_PARALLEL
					vicoDynamics::threading::FScopeWriterLock lock(OverlapCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

					// Cache the no hit results as well, chances are we won't move enough in a frame to miss a collision.
					FOverlapCacheEntry CacheValue = { nullptr, nullptr };
					OverlapCache.Add(MoveTemp(CacheKey), MoveTemp(CacheValue));
				}
			}
		}
	} // if Explicit Collision

	return Hit;
}

bool FVDCollisionQueryInterface::SweepSphere(float Radius, const vicoDynamics::FVector3Data& StartLocation, const vicoDynamics::FVector3Data& EndLocation, vicoDynamics::FQueryResults* Results, const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject)
{
	SCOPE_CYCLE_COUNTER(STAT_CISweep);

	bool Hit = false;
		
	const auto VDSimObj = static_cast<UVDSimulatedObject*>(SimObject->UserData);
	const auto VDComp = Cast<UBaseVDComponent>(VDSimObj->GetOuter());

	if (Simulation->ExplicitRigidBodyCollisionEnabled && VDComp && VDComp->UseExplicitRigidBodyCollision)
	{
		Hit = SweepAgainstExplicitRigidBodies(VDComp, VDSimObj, Radius, StartLocation, EndLocation, Results, Particle, SimObject);
	}
	else
	{
		const auto UStartLocation = VDVectorToUVector(StartLocation);
		const auto UEndLocation = VDVectorToUVector(EndLocation);

		FSweepCacheKey CacheKey = Particle;
		FSweepCacheEntry* ValuePtr = nullptr;
		if (bCachePerFrameResults)
		{
			SCOPE_CYCLE_COUNTER(STAT_CISweepCacheCheck);

#if VICODYNAMICS_USE_PARALLEL
			vicoDynamics::threading::FScopeReaderLock lock(SweepCacheCritSec);
//			if (SweepCache.Contains(CacheKey))
#endif // VICODYNAMICS_USE_PARALLEL
			ValuePtr = SweepCache.Find(CacheKey);
		}

		const auto World = Simulation->RunOnWorkerThread ? Simulation->SimulationThreadWorker->GetWorldRef() : Simulation->CurWorld;

		// Check the SweepCache first
		if (ValuePtr)
		{
			SCOPE_CYCLE_COUNTER(STAT_CISweepCacheHit);

			//const auto& Value = *ValuePtr;

			auto Actor = ValuePtr->Actor;
			auto Shape = ValuePtr->Shape;

			if (!Actor || !Shape) return false; // Early out if no collision was made this frame

			auto Position = ValuePtr->Position;
			auto Normal = ValuePtr->Normal;
			auto Depth = ValuePtr->Depth;

			// Ideally we would use Gworld->SweepSingleByChannel(), but it does not allow for passing in a Cached result to fastrack temporally coherent sweeps.
			// Instead we replicate that behavior by doing a manual geometry query sweep. Results should be identical.

			const auto Delta = UEndLocation - UStartLocation;
			const auto SweepDir = Delta.GetSafeNormal();
			const auto SweepDistance = Delta.Size();
			PxSweepHit SweepResult;
			static const auto QueryFlags = PxSceneQueryFlag::ePOSITION | PxSceneQueryFlag::eNORMAL | PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eMTD;
			Hit = PxGeometryQuery::sweep(U2PVector(SweepDir), SweepDistance, PxSphereGeometry(Radius), PxTransform(U2PVector(UStartLocation)), Shape->getGeometry().any(), Actor->getGlobalPose(), SweepResult, QueryFlags);
			if (Hit)
			{
				if (SweepResult.hadInitialOverlap())
				{
					Position = StartLocation;
					Normal = PVectorToVDVector(SweepResult.normal);
					Depth = SweepResult.distance;
				}
				else
				{
					Position = PVectorToVDVector(SweepResult.position);
					Normal = PVectorToVDVector(SweepResult.normal);
					Depth = SweepDistance - SweepResult.distance;
				}

				if (VDComp->bNotifyOnVDCollision)
				{
					UPrimitiveComponent* OtherComp = FPhysxUserData::Get<FBodyInstance>(Actor->userData)->OwnerComponent.Get();
					VDComp->AddPendingCollisionNotification(VDParticleToUVDParticle(Particle), OtherComp, VDVectorToUVector(Position), VDVectorToUVector(Normal));
				}

				Results->AddResult(Shape, Actor, Normal, Depth, Position);
			}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if (DrawCollisionDebug && !Simulation->RunOnWorkerThread)
			{
				const auto UNormal = VDVectorToUVector(Normal);
				auto ContactEdge = UEndLocation + (-UNormal * Radius);
				const auto UPosition = VDVectorToUVector(Position);

				DrawDebugPoint(World, ContactEdge, 10.f, FColor::Emerald, true, -1.f, 255);
				DrawDebugLine(World, ContactEdge, UPosition, FColor::Orange, true, -1.f, 255, .5f);
			}
#endif
		}
		else if (World)
		{
			const auto& Settings = VDSimObj->CollisionSettings;
			const auto VDSimParticle = VDParticleToUVDParticle(Particle);
			if (!VDSimParticle)
				return false;

			CollisionShape.SetSphere(Radius);			
			FHitResult HitResult;
			Hit = World->SweepSingleByChannel(HitResult, UStartLocation, UEndLocation, FQuat::Identity, Settings.ObjectType, CollisionShape, ColParams, FCollisionResponseParams(Settings.CollisionResponse.GetResponseContainer()));
			auto HitComponent = HitResult.GetComponent();
			if (Hit && HitComponent)
			{
				if (VDSimParticle->IsAttached && !Particle->GetIsFree() && HitComponent == VDSimParticle->Attachment.ComponentRef.GetComponent(VDComp->GetOwner()))
					return false;

				const auto BodyInstance = HitComponent->GetBodyInstance(HitResult.BoneName);
				const auto Actor = FPhysicsInterface::GetPxRigidActor_AssumesLocked(BodyInstance->GetActorReferenceWithWelding());

				const auto shapesCount = Actor->getNbShapes();
				if (shapesCount <= 0)
					return false;
				
				auto Position = UVectorToVDVector(HitResult.ImpactPoint);
				auto Normal = UVectorToVDVector(HitResult.ImpactNormal);
				const auto Depth = HitResult.bStartPenetrating ? HitResult.PenetrationDepth : (UEndLocation - UStartLocation).Size() - HitResult.Distance;
				
				physx::PxShape* Shape = nullptr;
				Actor->getShapes(&Shape, 1);
				Results->AddResult(Shape, Actor, Normal, Depth, Position);

				//UE_LOG(LogVICODynamics, All, TEXT("Normal: %s | Depth: %.3f | Position: %s"), *HitResult.ImpactNormal.ToString(), Depth, *HitResult.ImpactPoint.ToString());

				// Cache the Hit result
				if (bCachePerFrameResults)
				{
					if (!bCacheOnlyCollisionsWithStaticRigidBodies || (bCacheOnlyCollisionsWithStaticRigidBodies && Actor->is<PxRigidStatic>()))
					{
#if VICODYNAMICS_USE_PARALLEL
						vicoDynamics::threading::FScopeWriterLock lock(SweepCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

						FSweepCacheEntry CacheValue = { Actor, Shape, MoveTemp(Position), MoveTemp(Normal), Depth };
						SweepCache.Emplace(MoveTemp(CacheKey), MoveTemp(CacheValue));
					}
				}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
				if (DrawCollisionDebug && !Simulation->RunOnWorkerThread)
				{
					const auto ContactEdge = HitResult.ImpactPoint + (-HitResult.Normal * Radius);
					DrawDebugPoint(World, ContactEdge, 5.f, FColor::Purple, true);
					DrawDebugLine(World, ContactEdge, ContactEdge + (HitResult.ImpactNormal * -Depth), FColor::Red, true, -1.f, 0, .5f);
				}
#endif

				if (VDComp->bNotifyOnVDCollision)
					VDComp->AddPendingCollisionNotification(VDSimParticle, HitComponent, HitResult.ImpactPoint, HitResult.ImpactNormal);
			}
			else if (bCachePerFrameResults && bCacheNonHitsAlso)
			{
#if VICODYNAMICS_USE_PARALLEL
				vicoDynamics::threading::FScopeWriterLock lock(SweepCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

				// Cache the no hit results as well, chances are we won't move enough in a frame to miss a collision.
				FSweepCacheEntry CacheValue = { nullptr, nullptr, vicoDynamics::FVector3Data(), vicoDynamics::FVector3Data(), 0.f };
				SweepCache.Add(MoveTemp(CacheKey), MoveTemp(CacheValue));
			}
		}
	} // if Explicit Collision

	return Hit;
}

void FVDCollisionQueryInterface::LockSceneRead()
{
	if (Simulation.IsValid())
	{
		if (!Simulation->RunOnWorkerThread)
			GWorld->GetPhysicsScene()->GetPxScene()->lockRead();

		if (Simulation->ExplicitRigidBodyCollisionEnabled)
			Simulation->ColliderMappingCritSec.Lock();
	}
}

void FVDCollisionQueryInterface::LockSceneWrite()
{
	if (Simulation.IsValid() && !Simulation->RunOnWorkerThread)
	{
		GWorld->GetPhysicsScene()->GetPxScene()->lockWrite();
	}
}

void FVDCollisionQueryInterface::UnlockSceneRead()
{
	if (Simulation.IsValid())
	{
		if (!Simulation->RunOnWorkerThread)
			GWorld->GetPhysicsScene()->GetPxScene()->unlockRead();

		if (Simulation->ExplicitRigidBodyCollisionEnabled)
			Simulation->ColliderMappingCritSec.Unlock();
	}
}

void FVDCollisionQueryInterface::UnlockSceneWrite()
{
	if (Simulation.IsValid() && !Simulation->RunOnWorkerThread)
	{
		GWorld->GetPhysicsScene()->GetPxScene()->unlockWrite();
	}
}

void FVDCollisionQueryInterface::ResetCache()
{
	if (bCachePerFrameResults)
	{
		SCOPE_CYCLE_COUNTER(STAT_CICacheReset);
		OverlapCache.Empty();
		SweepCache.Empty();
	}
}

bool FVDCollisionQueryInterface::OverlapWithExplicitRigidBodies(UBaseVDComponent* VDComp, UVDSimulatedObject* VDSimObj, float Radius, const vicoDynamics::FVector3Data& Location, vicoDynamics::FQueryResults* Results, const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject)
{
//	SCOPE_CYCLE_COUNTER(STAT_CIExplicitOverlap);

	bool Hit = false;

	if (VDSimObj && VDComp->ExplicitColliders.Num())
	{
		const auto& Settings = VDSimObj->CollisionSettings;

		const PxSphereGeometry PxSphere = PxSphereGeometry(Radius);
		const PxTransform Transform = PxTransform(VDVectorToPVector(Location));

		const auto ULocation = VDVectorToUVector(Location);
		FOverlapCacheKey CacheKey = Particle;
		FOverlapCacheEntry* ValuePtr = nullptr;
		if (bCachePerFrameResults)
		{
#if VICODYNAMICS_USE_PARALLEL
			vicoDynamics::threading::FScopeReaderLock lock(OverlapCacheCritSec);
			if (OverlapCache.Contains(CacheKey))
#endif // VICODYNAMICS_USE_PARALLEL
				ValuePtr = OverlapCache.Find(CacheKey);
		}

		// Check the SweepCache first
		if (ValuePtr)
		{
			//SCOPE_CYCLE_COUNTER(STAT_CIOverlapCacheHit);

			//const auto& Value = *ValuePtr;

			auto Actor = ValuePtr->Actor;
			auto Shape = ValuePtr->Shape;

			if (!Actor || !Shape) return false; // Early out if no collision was made this frame

			Hit = PxGeometryQuery::overlap(PxSphere, Transform, Shape->getGeometry().any(), Actor->getGlobalPose());
			if (Hit)
			{
				Results->AddResult(Shape, Actor);
			}
		}
		else
		{
			for (int32 i = 0; i < VDComp->ExplicitColliders.Num(); i++)
			{
				auto& ColliderInfo = VDComp->ExplicitColliders[i];
				if (!ColliderInfo.IsValid() || ColliderInfo.SkipChecks) continue;

				UPrimitiveComponent* Collider = ColliderInfo.Collider;

				// Check if collision is enabled
				const auto ColliderCollisionEnabled = Collider->GetCollisionEnabled();
				if (ColliderCollisionEnabled == ECollisionEnabled::QueryOnly || ColliderCollisionEnabled == ECollisionEnabled::NoCollision) continue;

				// Check if we can collide
				const auto ColliderObjectType = Collider->GetCollisionObjectType();
				if (Settings.CollisionResponse.GetResponse(ColliderObjectType) != ECR_Block || Collider->GetCollisionResponseToChannel(Settings.ObjectType) != ECR_Block) continue;

				if (auto skelMesh = Cast<USkeletalMeshComponent>(Collider))
				{
					TArray<FName> BoneNames;
					skelMesh->GetBoneNames(BoneNames);
					for (int32 j = 0; j < BoneNames.Num(); j++)
					{
						const auto BI = skelMesh->GetBodyInstance(BoneNames[j]);
						if (!BI) continue;

						auto Actor = FPhysicsInterface::GetPxRigidActor_AssumesLocked(BI->GetPhysicsActorHandle());
						auto PxActorPose = Actor->getGlobalPose();

						TArray<FPhysicsShapeHandle> allShapes;
						auto shapes = BI->GetAllShapes_AssumesLocked(allShapes);
						if (shapes <= 0 || !allShapes[0].IsValid()) continue;

						auto Shape = allShapes[0].Shape; // Check against first shape only

						{
	//						SCOPE_CYCLE_COUNTER(STAT_CIExplicitOverlapPxGeom);
							Hit = PxGeometryQuery::overlap(PxSphere, Transform, Shape->getGeometry().any(), PxActorPose);
						}
							
						if (Hit)
						{
							Results->AddResult(Shape, Actor);
							if (bCachePerFrameResults)
							{
								if (bCacheOnlyCollisionsWithStaticRigidBodies && !Actor->is<PxRigidStatic>())
									break;
#if VICODYNAMICS_USE_PARALLEL
								vicoDynamics::threading::FScopeWriterLock lock(OverlapCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

								// Cache the Hit result
								FOverlapCacheEntry CacheValue = { Actor, Shape };
								OverlapCache.Emplace(CacheKey, MoveTemp(CacheValue));
							}

							break;
						}
						else if (bCachePerFrameResults && bCacheNonHitsAlso)
						{
#if VICODYNAMICS_USE_PARALLEL
							vicoDynamics::threading::FScopeWriterLock lock(OverlapCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

							// Cache the no hit results as well, chances are we won't move enough in a frame to miss a collision.
							FOverlapCacheEntry CacheValue = { nullptr, nullptr };
							OverlapCache.Emplace(CacheKey, MoveTemp(CacheValue));
						}
					}
				}
				else
				{
					auto Actor = ColliderInfo.PxActor;
					auto PxActorPose = Actor->getGlobalPose();

					for (int32 sIdx = 0; sIdx < ColliderInfo.PxShapes.Num(); sIdx++)
					{
						auto Shape = ColliderInfo.PxShapes[sIdx];

						{
		//					SCOPE_CYCLE_COUNTER(STAT_CIExplicitOverlapPxGeom);
							Hit = PxGeometryQuery::overlap(PxSphere, Transform, Shape->getGeometry().any(), PxActorPose);
						}

						if (Hit)
						{
							Results->AddResult(Shape, Actor);

							if (bCachePerFrameResults)
							{
								if (bCacheOnlyCollisionsWithStaticRigidBodies && !Actor->is<PxRigidStatic>())
									break;
#if VICODYNAMICS_USE_PARALLEL
								vicoDynamics::threading::FScopeWriterLock lock(OverlapCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

								// Cache the Hit result
								FOverlapCacheEntry CacheValue = { Actor, Shape };
								OverlapCache.Emplace(CacheKey, MoveTemp(CacheValue));
							}

							break;
						}
						else if (bCachePerFrameResults && bCacheNonHitsAlso)
						{
#if VICODYNAMICS_USE_PARALLEL
							vicoDynamics::threading::FScopeWriterLock lock(OverlapCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

							// Cache the no hit results as well, chances are we won't move enough in a frame to miss a collision.
							FOverlapCacheEntry CacheValue = { nullptr, nullptr };
							OverlapCache.Emplace(CacheKey, MoveTemp(CacheValue));
						}
					}
				}
			}
		}
	}

	return Hit;
}

bool FVDCollisionQueryInterface::SweepAgainstExplicitRigidBodies(UBaseVDComponent* VDComp, UVDSimulatedObject* VDSimObj, float Radius, const vicoDynamics::FVector3Data& StartLocation, const vicoDynamics::FVector3Data& EndLocation, vicoDynamics::FQueryResults* Results, const vicoDynamics::FSimulatedParticle* Particle, const vicoDynamics::FSimulatedObject* SimObject)
{
	//SCOPE_CYCLE_COUNTER(STAT_CIExplicitSweep);

	bool Hit = false;

	if (VDSimObj && VDComp->ExplicitColliders.Num())
	{
		const auto UStartLocation = VDVectorToUVector(StartLocation);
		const auto UEndLocation = VDVectorToUVector(EndLocation);
		const auto Delta = UEndLocation - UStartLocation;
		const auto SweepDir = Delta.GetSafeNormal();
		const auto SweepDistance = Delta.Size();
		const PxVec3 PxSweepDir = U2PVector(SweepDir);
		const PxSphereGeometry PxSphere = PxSphereGeometry(Radius);
		const PxTransform PxStartTransform = PxTransform(U2PVector(UStartLocation));
		PxSweepHit SweepResult;
		const auto QueryFlags = PxSceneQueryFlag::eMTD;

		const auto& Settings = VDSimObj->CollisionSettings;
		auto World = Simulation->RunOnWorkerThread ? Simulation->SimulationThreadWorker->GetWorldRef() : GWorld;

		FSweepCacheKey CacheKey = Particle;
		FSweepCacheEntry* ValuePtr = nullptr;
		if (bCachePerFrameResults)
		{
#if VICODYNAMICS_USE_PARALLEL
			vicoDynamics::threading::FScopeReaderLock lock(SweepCacheCritSec);
			if (SweepCache.Contains(CacheKey))
#endif // VICODYNAMICS_USE_PARALLEL
				ValuePtr = SweepCache.Find(CacheKey);
		}
		
		// Check the SweepCache first
		if (ValuePtr)
		{
	//		SCOPE_CYCLE_COUNTER(STAT_CISweepCacheHit);

			//const auto& Value = *ValuePtr;

			auto Actor = ValuePtr->Actor;
			auto Shape = ValuePtr->Shape;

			if (!Actor || !Shape) return false; // Early out if no collision was made this frame

			auto Position = ValuePtr->Position;
			auto Normal = ValuePtr->Normal;
			auto Depth = ValuePtr->Depth;

			// Ideally we would use Gworld->SweepSingleByChannel(), but it does not allow for passing in a Cached result to fastrack temporally coherent sweeps.
			// Instead we replicate that behavior by doing a manual geometry query sweep. Results should be identical.

			Hit = PxGeometryQuery::sweep(PxSweepDir, SweepDistance, PxSphere, PxStartTransform, Shape->getGeometry().any(), Actor->getGlobalPose(), SweepResult, QueryFlags);
			if (Hit)
			{
				if (SweepResult.hadInitialOverlap())
				{
					Position = StartLocation;
					Normal = PVectorToVDVector(SweepResult.normal);
					Depth = SweepResult.distance;
				}
				else
				{
					Position = PVectorToVDVector(SweepResult.position);
					Normal = PVectorToVDVector(SweepResult.normal);
					Depth = SweepDistance - SweepResult.distance;
				}

				Results->AddResult(Shape, Actor, Normal, Depth, Position);
			}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if (DrawCollisionDebug && !Simulation->RunOnWorkerThread)
			{
				const auto UNormal = VDVectorToUVector(Normal);
				auto ContactEdge = UEndLocation + (-UNormal * Radius);
				const auto UPosition = VDVectorToUVector(Position);

				DrawDebugPoint(World, ContactEdge, 10.f, FColor::Emerald, true, -1.f, 255);
				DrawDebugLine(World, ContactEdge, UPosition, FColor::Orange, true, -1.f, 255, .5f);
			}
#endif
		}
		else
		{
			for (int32 i = 0; i < VDComp->ExplicitColliders.Num(); i++)
			{
				auto& ColliderInfo = VDComp->ExplicitColliders[i];

				if (!ColliderInfo.IsValid() || ColliderInfo.SkipChecks) continue;

				auto Collider = ColliderInfo.Collider;

				// Check if collision is enabled
				const auto ColliderCollisionEnabled = Collider->GetCollisionEnabled();
				if (ColliderCollisionEnabled == ECollisionEnabled::QueryOnly || ColliderCollisionEnabled == ECollisionEnabled::NoCollision) continue;

				// Check if we can collide
				const auto ColliderObjectType = Collider->GetCollisionObjectType();
				if (Settings.CollisionResponse.GetResponse(ColliderObjectType) != ECR_Block || Collider->GetCollisionResponseToChannel(Settings.ObjectType) != ECR_Block) continue;

				if (auto skelMesh = Cast<USkeletalMeshComponent>(Collider))
				{
					TArray<FName> BoneNames;
					skelMesh->GetBoneNames(BoneNames);
					for (int32 j = 0; j < BoneNames.Num(); j++)
					{
						const auto BI = skelMesh->GetBodyInstance(BoneNames[j]);
						if (!BI) continue;

						auto Actor = FPhysicsInterface::GetPxRigidActor_AssumesLocked(BI->GetPhysicsActorHandle());

						TArray<FPhysicsShapeHandle> allShapes;
						auto shapes = BI->GetAllShapes_AssumesLocked(allShapes);
						if (shapes <= 0 && !allShapes[0].IsValid())
						{
							Hit = false;
							continue;
						}

						auto Shape = allShapes[0].Shape; // Check against first shape only
						{
		//					SCOPE_CYCLE_COUNTER(STAT_CIExplicitSweepPxGeom);
							Hit = PxGeometryQuery::sweep(PxSweepDir, SweepDistance, PxSphere, PxStartTransform, Shape->getGeometry().any(), Actor->getGlobalPose(), SweepResult, QueryFlags);
						}
						if (Hit)
						{
							vicoDynamics::FVector3Data Position, Normal;
							float Depth;

							if (SweepResult.hadInitialOverlap())
							{
								Position = StartLocation;
								Normal = PVectorToVDVector(SweepResult.normal);
								Depth = -SweepResult.distance;
							}
							else
							{
								Position = PVectorToVDVector(SweepResult.position);
								Normal = PVectorToVDVector(SweepResult.normal);
								Depth = SweepDistance - SweepResult.distance;
							}

							Results->AddResult(Shape, Actor, Normal, Depth, Position);
							
							// Cache the Hit result
							if (bCachePerFrameResults)
							{
								bool CacheThisResult = true;
								if (bCacheOnlyCollisionsWithStaticRigidBodies && !Actor->is<PxRigidStatic>())
									CacheThisResult = false;

								if (CacheThisResult)
								{
#if VICODYNAMICS_USE_PARALLEL
									vicoDynamics::threading::FScopeWriterLock lock(SweepCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

									FSweepCacheEntry CacheValue = { Actor, Shape, Position, Normal, Depth };
									SweepCache.Emplace(CacheKey, MoveTemp(CacheValue));
								}
							}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
							if (DrawCollisionDebug && !Simulation->RunOnWorkerThread)
							{
								const auto ContactEdge = P2UVector(SweepResult.position + (-SweepResult.normal * Radius));
								DrawDebugPoint(World, ContactEdge, 5.f, FColor::Purple, true);
								DrawDebugLine(World, ContactEdge, ContactEdge + P2UVector(SweepResult.normal * -Depth), FColor::Red, true, -1.f, 0, .5f);
							}
#endif
							break;
						}
					}
				}
				else
				{
					auto Actor = ColliderInfo.PxActor;

					for (int32 sIdx = 0; sIdx < ColliderInfo.PxShapes.Num(); sIdx++)
					{
						auto Shape = ColliderInfo.PxShapes[sIdx];
						{
//							SCOPE_CYCLE_COUNTER(STAT_CIExplicitSweepPxGeom);
							Hit = PxGeometryQuery::sweep(PxSweepDir, SweepDistance, PxSphere, PxStartTransform, Shape->getGeometry().any(), Actor->getGlobalPose(), SweepResult, QueryFlags);
						}
						if (Hit)
						{
							vicoDynamics::FVector3Data Position, Normal;
							float Depth;

							if (SweepResult.hadInitialOverlap())
							{
								Position = StartLocation;
								Normal = PVectorToVDVector(SweepResult.normal);
								Depth = -SweepResult.distance;
							}
							else
							{
								Position = PVectorToVDVector(SweepResult.position);
								Normal = PVectorToVDVector(SweepResult.normal);
								Depth = SweepDistance - SweepResult.distance;
							}

							Results->AddResult(Shape, Actor, Normal, Depth, Position);
							
							// Cache the Hit result
							if (bCachePerFrameResults)
							{
								bool CacheThisResult = true;
								if (bCacheOnlyCollisionsWithStaticRigidBodies && !Actor->is<PxRigidStatic>())
									CacheThisResult = false;

								if (CacheThisResult)
								{
#if VICODYNAMICS_USE_PARALLEL
									vicoDynamics::threading::FScopeWriterLock lock(SweepCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

									FSweepCacheEntry CacheValue = { Actor, Shape, Position, Normal, Depth };
									SweepCache.Emplace(CacheKey, MoveTemp(CacheValue));
								}
							}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
							if (DrawCollisionDebug && !Simulation->RunOnWorkerThread)
							{
								const auto ContactEdge = P2UVector(SweepResult.position + (-SweepResult.normal * Radius));
								DrawDebugPoint(World, ContactEdge, 5.f, FColor::Purple, true);
								DrawDebugLine(World, ContactEdge, ContactEdge + P2UVector(SweepResult.normal * -Depth), FColor::Red, true, -1.f, 0, .5f);
							}
#endif					
							break;
						}
						else if (bCachePerFrameResults && bCacheNonHitsAlso)
						{
#if VICODYNAMICS_USE_PARALLEL
							vicoDynamics::threading::FScopeWriterLock lock(SweepCacheCritSec);
#endif // VICODYNAMICS_USE_PARALLEL

							// Cache the no hit results as well, chances are we won't move enough in a frame to miss a collision.
							FSweepCacheEntry CacheValue = { nullptr, nullptr, vicoDynamics::FVector3Data(), vicoDynamics::FVector3Data(), 0.f };
							SweepCache.Emplace(CacheKey, MoveTemp(CacheValue));
						}
					}
				}
			}
		}
	}

	return Hit;
}
