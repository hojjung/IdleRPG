// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

/*
  The structs in this header are only used for convenient access to Particle data, therefor avoiding exporting complex Vector types.
*/

#pragma once

#include "VICODynamics.h"
#include <stdint.h>

namespace vicoDynamics
{
	/**
	 * A super simple struct to represent a 3D vector with three floats. Used for copying data between the Simulation and clients avoiding exporting complex Vector types.
	 * A client could setup a union using FVector3Data and its built in Vector type if the size is the same for even more flexibility.
	 */
	struct FVector3Data
	{
		/**
		 * Constructs a zeroed out Vector.
		 */
		FVector3Data() : X(0.f), Y(0.f), Z(0.f)
		{}

		FVector3Data(const FVector3Data& Other) : X(Other.X), Y(Other.Y), Z(Other.Z)
		{}

		/**
		 * Constructs a Vector out of the supplied values.
		 */
		FVector3Data(float x, float y, float z) : X(x), Y(y), Z(z)
		{}

		float X;
		float Y;
		float Z;
	};

	/**
	 * Holds all the Simulation statistics for profiling.
	 */
	struct FSimulationStats
	{
		FSimulationStats() :
			TotalSimulationTimeMS(0),
			VelocityComputationTimeMS(0),
			ConstraintSolverTimeMS(0),
			VelocityCorrectionTimeMS(0),
			CollisionTimeMS(0),
			ParticleCollisionBroadPhaseTimeMS(0),
			ParticleCollisionNarrowPhaseTimeMS(0),
			RigidBodyCollisionTimeMS(0),
			CollisionSpatialHashResetTimeMS(0),
			ParticleCount(0),
			ConstraintCount(0),
			SolveCallsSatisfied(0),
			SolveCallsUnsatisfied(0)
		{}

		/**
		 * Total time taken for a complete Simulation Tick in the last frame.
		 */
		double TotalSimulationTimeMS;

		/**
		 * Time taken to compute the velocities for all the particles in the last frame.
		 */
		double VelocityComputationTimeMS;

		/**
		 * Time taken to solve the constraints for all the particles in the last frame.
		 */
		double ConstraintSolverTimeMS;

		/**
		 * Time taken to correct the velocity for all the particles in the last frame.
		 */
		double VelocityCorrectionTimeMS;

		/**
		 * Time taken to detect/resolve all the collisions for the particles in the last frame. Includes Particle/Particle and Particle/RigidBody.
		 */
		double CollisionTimeMS;

		/**
		 * Time taken to detect all the Particle/Particle collisions in the last frame.
		 */
		double ParticleCollisionBroadPhaseTimeMS;
		
		/**
		 * Time taken to resolve all the Particle/Particle collisions in the last frame.
		 */
		double ParticleCollisionNarrowPhaseTimeMS;

		/**
		 * Time taken to detect/resolve all the Particle/RigidBody collisions in the last frame.
		 */
		double RigidBodyCollisionTimeMS;

		/**
		 * Time taken by CollisionSpatialHash.Reset();
		 */
		double CollisionSpatialHashResetTimeMS;

		/**
		 * Total number of particles in the simulation.
		 */
		uint32_t ParticleCount;

		/**
		 * Total number of constraints in the simulation.
		 */
		uint32_t ConstraintCount;

		uint32_t SolveCallsSatisfied;
		uint32_t SolveCallsUnsatisfied;
	};
}