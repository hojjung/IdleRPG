// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "SimulatedParticle.h"
#include "ParticleConstraints.h"
#include "Collision.h"

#include <cstdlib>

namespace vicoDynamics
{
	/**
	 * Used to associate a group(s) of Particles and Constraints to a single entity for easier management.
	 */
	class FSimulatedObject
	{
	protected:
		FSimulatedObject() : UserData(nullptr) {}
		virtual ~FSimulatedObject() {};

	public:
		/**
		 * Removes all associated Particles and Constraints from the simulation and frees this instance.
		 */
		virtual void Release() = 0;

		/**
		 * Removes all associated Particles and Constraints from the simulation but keeps this instance alive.
		 */
		virtual void RemoveFromSimulation() = 0;

		/**
		 * @return The number of Particles owned by this Simulated Object.
		 */
		virtual size_t GetParticleCount() const = 0;

		/**
		 * @return The number of Constraints owned by this Simulated Object.
		 */
		virtual size_t GetConstraintCount() const = 0;

		/**
		 * Fills the OutParticles array with pointers to the Particles owned by this Simulated Object.
		 * 
		 * @param OutParticles - The array to fill. Must be pre-allocated.
		 * @param Count - The desired number of Particles to retrieve. Must be between 1 and GetParticleCount(), inclusive.
		 * @param Offset - The offset in the internal Particle buffer to start at. Not the OutParticles offset. Must be between 0 and GetParticleCount() - 1, inclusive.
		 *
		 * @return The number of Particles retrieved. 0 if an error occurred.
		 */
		virtual size_t GetParticles(FSimulatedParticle** OutParticles, size_t Count, size_t Offset = 0) const = 0;

		/**
		 * Fills the OutConstraints array with pointers to the Constraints owned by this Simulated Object.
		 *
		 * @param OutConstraints - The array to fill. Must be pre-allocated.
		 * @param Count - The desired number of Constraints to retrieve. Must be between 1 and GetConstraintCount(), inclusive.
		 * @param Offset - The offset in the internal Constraint buffer to start at. Not the OutConstraints offset. Must be between 0 and GetConstraintCount() - 1, inclusive.
		 *
		 * @return The number of Constraints retrieved. 0 if an error occurred.
		 */
		virtual size_t GetConstraints(FParticleSpringConstraint** OutConstraints, size_t Count, size_t Offset = 0) const = 0;

		/**
		 * @return The minimum distance at which a constraint is considered satisfied.
		 */
		virtual float GetMinimumConstraintSatisfactionDistance() const = 0;

		/**
		 * @param NewMinimumDistance - The minimum distance at which a constraint is considered satisfied. 
		 *							   If the distance is less than this value, the constraint is skipped for the current Substep. Default value is .007f.
		 *							   Increasing this can help performance but may introduce jumpy constraints. Reducing this value will increase the accuracy of constraints at the cost of performance.
		 */
		virtual void SetMinimumConstraintSatisfactionDistance(float NewMinimumDistance) = 0;

		/*
		 * @return The amount to scale Gravity by during the Velocity phase. Gets applied to all Particles of this SimulatedObject.
		 */
		virtual float GetGravityScale() const = 0;

		/*
		 * @param Scale - The amount to scale Gravity by during the Velocity phase. Gets applied to all Particles of this SimulatedObject.
		 */
		virtual void SetGravityScale(float Scale) = 0;

		/*
		* @return The Wind Vector affecting this Simulated Object.
		*/
		virtual FVector3Data GetAffectiveWindVector() const = 0;

		/*
		* @param The new Wind Vector affecting this Simulated Object. NOTE: NewWindVector must be a combined vector of direction and speed.
		*/
		virtual void SetAffectiveWindVector(const FVector3Data& NewWindVector) = 0;
		
		/**
		 * @return Whether the particles owned by this instance are considered for collision.
		 */
		virtual bool GetCollisionEnabled() const = 0;

		/**
		 * @param Enabled - If true, particles owned by this instance will be considered for collision.
		 */
		virtual void SetCollisionEnabled(bool Enabled) = 0;

		/**
		 * @return Whether the relative velocity is not taken into account during RigidBody collision. Advanced option, helps in some edge cases.
		 */
		virtual bool GetSkipRelativeVelocityCheckDuringCollision() const = 0;

		/**
		 * @param Skip - If true, the relative velocity is not taken into account during RigidBody collision. Advanced option, helps in some edge cases.
		 */
		virtual void SetSkipRelativeVelocityCheckDuringCollision(bool Skip) = 0;
		
	public:
		/**
		 * Any user data to be embedded with this SimObject
		 */
		void* UserData;

		/**
		 * Used by the collision interface for filtering.
		 */
		FCollisionFilter CollisionFilter;
	};
}