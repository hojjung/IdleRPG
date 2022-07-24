// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include <stdint.h>
#include "DataContainers.h"

namespace vicoDynamics
{
	class FSimulatedObject;
	struct FSimulatedParticle;

	/**
	 * [NOT USED CURRENTLY]
	 * A general purpose user-definable struct for collision filtering
	 * Currently setup for compatibility with PhysX.
	 */
	struct FCollisionFilter
	{
		FCollisionFilter()
		{
			Word0 = Word1 = Word2 = Word3 = 0;
		}

		FCollisionFilter(uint32_t word0, uint32_t word1, uint32_t word2, uint32_t word3) : Word0(word0), Word1(word1), Word2(word2), Word3(word3)
		{}

		uint32_t Word0;
		uint32_t Word1;
		uint32_t Word2;
		uint32_t Word3;
	};

	class FQueryResults
	{
	public:
        virtual ~FQueryResults() {}
		virtual void AddResult(void* Shape, void* Actor) = 0;
		virtual void AddResult(void* Shape, void* Actor, const FVector3Data& Normal, float Depth, const FVector3Data& Location) = 0;
	};

	class FCollisionQueryInterface
	{
	public:
        virtual ~FCollisionQueryInterface() {}
        virtual bool OverlapSphere(float Radius, const FVector3Data& Location, FQueryResults* Results, const FSimulatedParticle* Particle, const FSimulatedObject* SimObject) = 0;
		virtual bool SweepSphere(float Radius, const FVector3Data& StartLocation, const FVector3Data& EndLocation, FQueryResults* Results, const FSimulatedParticle* Particle, const FSimulatedObject* SimObject) = 0;
		virtual void LockSceneRead() = 0;
		virtual void LockSceneWrite() = 0;
		virtual void UnlockSceneRead() = 0;
		virtual void UnlockSceneWrite() = 0;
	};

	enum class EQueryShapeType
	{
		Box,
		Sphere
	};

	struct FQueryShape
	{
	protected:
		FQueryShape(EQueryShapeType Type) : ShapeType(Type) 
		{}

	public:
		EQueryShapeType GetType() const { return ShapeType; }

	protected:
		EQueryShapeType ShapeType;
	};

	struct FQueryBoxShape : public FQueryShape
	{
		FQueryBoxShape() : FQueryShape(EQueryShapeType::Box)
		{}
		FQueryBoxShape(FVector3Data halfExtents) : FQueryShape(EQueryShapeType::Box), HalfExtents(halfExtents)
		{}

		FVector3Data HalfExtents;
	};

	struct FQuerySphereShape : public FQueryShape
	{
		FQuerySphereShape() : FQueryShape(EQueryShapeType::Sphere), Radius(0.f)
		{}
		FQuerySphereShape(float radius) : FQueryShape(EQueryShapeType::Sphere), Radius(radius)
		{}

		float Radius;
	};

	struct FQueryOverlapBuffer
	{
		FQueryOverlapBuffer(FSimulatedParticle** resultsBuffer, uint32_t maxOverlaps) : Overlaps(resultsBuffer), MaxNumberOfOverlaps(maxOverlaps), NumberOfOverlapsRecorded(0)
		{}

		FSimulatedParticle** Overlaps;
		uint32_t MaxNumberOfOverlaps;
		uint32_t NumberOfOverlapsRecorded;
	};

	struct FQueryFilter
	{
		FQueryFilter() : NumberOfSimObjectsToIgnore(0), SimObjectsToIgnore(nullptr)
		{}

		FQueryFilter(uint32_t numSimObjectsToIgnore, FSimulatedObject** simObjectsToIgnore) : NumberOfSimObjectsToIgnore(numSimObjectsToIgnore), SimObjectsToIgnore(simObjectsToIgnore)
		{}

		uint32_t NumberOfSimObjectsToIgnore;
		FSimulatedObject** SimObjectsToIgnore;
	};
}