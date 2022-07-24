// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "DataContainers.h"

#include <stdint.h>

namespace vicoDynamics
{
	namespace EParticleFlag
	{
		enum Type
		{
			None = 0,
			Collision_IncludeNeighbors = 1 << 0,
			Collision_IgnoreConstrainedTo = 1 << 1,
			Collision_TestAsSphere = 1 << 2,
			Collision_DisableParticleCollision = 1 << 3,
			Collision_DisableRigidBodyCollision = 1 << 4,
			EParticleFlag_Max = 1 << 5
		};
	}
	/**
	 * The base class for the simulated Particles.
	 */
	struct FSimulatedParticle
	{
	protected:
		FSimulatedParticle() : UserData(nullptr) {}
		virtual ~FSimulatedParticle() {}

	public:
		
		/**
		 * @return If true, the particle is free to be simulated. Otherwise it is fixed to its Location.
		 */
		virtual bool GetIsFree() const = 0;

		/**
		 * @param IsFree - If true, the particle is free to be simulated. Otherwise it is fixed to its Location.
		 */
		virtual void SetIsFree(bool IsFree) = 0;

		/**
		 * @return The Particles collision Radius.
		 */
		virtual float GetRadius() const = 0;
		
		/**
		 * @param Radius - New Particle collision Radius.
		 */
		virtual void SetRadius(float Radius) = 0;

		/**
		 * @return The Particles current location.
		 */
		virtual FVector3Data GetLocation() const = 0;

		/**
		* @param NewLocation - The Particles new location.
		*/
		virtual void SetLocation(const FVector3Data& NewLocation) = 0;

		/**
		 * @return The Particles current location. NOTE: Use this ONLY if you know what you are doing!
		 */
		virtual FVector3Data GetProjectedLocation() const = 0;

		/**
		 * @param NewLocation - The Particles new Projected location. NOTE: Use this ONLY if you know what you are doing!
		 */
		virtual void SetProjectedLocation(const FVector3Data& NewProjectedLocation) = 0;

		/**
		 * @return The Particles current velocity. Will be zero if IsFree is false.
		 */
		virtual FVector3Data GetVelocity() const = 0;
		
		/**
		 * @param NewVelocity - The Particles new velocity.
		 */
		virtual void SetVelocity(const FVector3Data& NewVelocity) = 0;

		/**
		 * @return The Particles current friction
		 */
		virtual float GetFriction() const = 0;

		/**
		 * @param NewFriction - The Particles new friction.
		 */
		virtual void SetFriction(float NewFriction) = 0;

		/**
		 * @return The Particles current restitution
		 */
		virtual float GetRestitution() const = 0;

		/**
		 * @param NewRestitution - The Particles new restitution.
		 */
		virtual void SetRestitution(float NewRestitution) = 0;

		/**
		 * @return The Particles current mass
		 */
		virtual float GetMass() const = 0;

		/**
		 * @param NewMass - The Particles new mass.
		 */
		virtual void SetMass(float NewMass) = 0;

		/**
		* @return The Particles current drag coefficient.
		*/
		virtual float GetDragCoefficient() const = 0;

		/**
		 * @param NewDragCoefficient -The Particles new drag coefficient.
		 */
		virtual void SetDragCoefficient(float NewDragCoefficient) = 0;

		virtual bool GetIsOccludedFromWind() const = 0;
		virtual void SetIsOccludedFromWind(bool IsOccluded) = 0;

		/**
		 * @return The Particles unique identifier.
		 */
		virtual uint_fast32_t GetIndex() const = 0;

		/**
		 * @return True if the particle is in the simulation
		 */
		virtual bool IsValid() const = 0;

		virtual EParticleFlag::Type GetFlags() const = 0;

		virtual void SetFlags(EParticleFlag::Type Flags) = 0;

		virtual void RemoveFlags(EParticleFlag::Type Flags) = 0;

		/**
		 * @param Force - The Force to exert on this Particle, Inverse Mass is applied automatically.
		 */
		virtual void AddForce(const FVector3Data& Force) = 0;

	public:
		/**
		 * Any user data to be embedded with this Particle
		 */
		void* UserData;
	};
}