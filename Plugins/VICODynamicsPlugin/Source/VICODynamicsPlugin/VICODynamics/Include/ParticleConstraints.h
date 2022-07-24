// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "SimulatedParticle.h"

namespace vicoDynamics
{
	/**
	 * The base class for any constraint between two Particles.
	 */
	struct FTwoParticleConstraint
	{
	protected:
		FTwoParticleConstraint(int_fast32_t Particle1Index, int_fast32_t Particle2Index) : UserData(nullptr) {}
		virtual ~FTwoParticleConstraint() {}

	public:
		/**
		 * @return The unique ID of the first particle affected by this constraint.
		 */
		virtual int_fast32_t GetParticle1Index() const = 0;

		/**
		 * @return The unique ID of the second particle affected by this constraint.
		 */
		virtual int_fast32_t GetParticle2Index() const = 0;

		/**
		 * @return The pointer to the first particle affected by this constraint.
		 */
		virtual FSimulatedParticle* GetParticle1() = 0;

		/**
		 * @return The pointer to the second particle affected by this constraint.
		 */
		virtual FSimulatedParticle* GetParticle2() = 0;

		/**
		 * @return The Constraints unique identifier.
		 */
		virtual int_fast32_t GetIndex() const = 0;

		/**
		 * @return True if this constraint is valid.
		 */
		virtual bool IsValid() const = 0;

		virtual void RefreshCachedData() = 0;

	public:
		/**
		 * Any user data to be embedded with this Constraint
		 */
		void* UserData;
	};

	/**
	 * The spring constraint, soft distance constraint.
	 */
	struct FParticleSpringConstraint : public FTwoParticleConstraint
	{
	protected:
		FParticleSpringConstraint(int_fast32_t Particle1Index, int_fast32_t Particle2Index) : FTwoParticleConstraint(Particle1Index, Particle2Index) {}
		virtual ~FParticleSpringConstraint() {}

	public:
		/**
		 * @return The current rest distance for this spring constraint.
		 */
		virtual float GetRestDistance() const = 0;

		/**
		 * @param NewRestDistance - The desired new rest distance for this spring constraint.
		 */
		virtual void SetRestDistance(float NewRestDistance) = 0;

		/**
		 * @return The current stiffness for this spring constraint. Range: 0 to 1.
		 */
		virtual float GetStiffness() const = 0;

		/**
		 * @param NewStiffness - The desired new stiffness for this spring constraint. Range: 0 to 1.
		 */
		virtual void SetStiffness(float NewStiffness) = 0;		
	};

	struct FExternalConstraint
	{
		virtual ~FExternalConstraint() {}

		virtual void Solve(float DeltaTime) = 0;
	};
}