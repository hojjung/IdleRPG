// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "VICODynamics.h"
#include "SimulatedParticle.h"
#include "ParticleConstraints.h"
#include "SimulatedObject.h"

namespace physx
{
	class PxScene;
}

namespace vicoDynamics
{
	/**
	 * Main Simulation class.
	 */
	class FSimulation
	{
	protected:
		FSimulation() {}
		virtual ~FSimulation() {}

	public:
		/**
		 * Destroys this simulation.
		 */
		virtual void Release() = 0;
        
        virtual bool GetSimulationReleased() const = 0;

		/**
		 * @return The passed in PxScene to use for RigidBody collision. May be null.
		 */
		virtual physx::PxScene* GetPhysXScene() const = 0;

		/**
		 * @return The passed in async PxScene to use for RigidBody collision. May be null. UE4 specific.
		 */
		virtual physx::PxScene* GetPhysXAsyncScene() const = 0;

		/**
		 * Simulates the Particle physics.
		 *
		 * @param DeltaTime - Frame time for this frame. Used to determine the simulation steps to take based on the set iteration count.
		 */
		virtual void TickSimulation(float DeltaTime) = 0;

		/**
		 * @return The desired number of simulation iterations to perform on each Simulation tick.
		 */
		virtual int_fast32_t GetSimulationIterations() const = 0;

		/**
		 * @param Iterations - The desired number of simulation iterations to perform on each Simulation tick. 
		 *					  More iterations stabilize the simulation but come at a performance cost.
		 */
		virtual void SetSimulationIterations(int_fast32_t Iterations) = 0;

		/**
		 * @return The time step at which the simulation is running. 1/60fps by default.
		 */
		virtual float GetSimulationTimeStep() const = 0;

		/**
		 * @param TimeStep - The time step at which the simulation will run in. 1/60fps by default.
		 */
		virtual void SetSimulationTimeStep(float TimeStep) = 0;

		/**
		 * @return The desired number of constraint solver iterations to perform on each Simulation iteration.
		 */
		virtual int_fast32_t GetConstraintIterations() const = 0;

		/**
		 * @param Iterations - The desired number of constraint solver iterations to perform on each Simulation iteration. 
		 *		              More iterations stabilize the constraint simulation but come at a performance cost. Actual constraint iteration count per tick is: SimIteration * ConstraintIterations
		 */
		virtual void SetConstraintIterations(int_fast32_t Iterations) = 0;

		/**
		 * @return The Gravity vector.
		 */
		virtual FVector3Data GetGravity() const = 0;

		/**
		 * @param NewGravity - The desired Gravity vector.
		 */
		virtual void SetGravity(const FVector3Data& NewGravity) = 0;
		
		virtual float GetAirDensity() const = 0;
		virtual void SetAirDensity(float Density) = 0;

		virtual bool GetConstrainToPlane() const = 0;
		virtual void SetConstrainToPlane(bool bConstrain) = 0;
		virtual FVector3Data GetConstrainPlaneOrigin() const = 0;
		virtual void SetConstrainPlaneOrigin(const FVector3Data& Origin) = 0;
		virtual FVector3Data GetConstrainPlaneNormal() const = 0;
		virtual void SetConstrainPlaneNormal(const FVector3Data& Normal) = 0;

		/**
		 * @return true if Particle/Particle collision is enabled.
		 */
		virtual bool GetSelfCollisionEnabled() const = 0;

		/**
		 * @param bEnabled - If true, Particle/Particle collision is enabled. Affects performance.
		 */
		virtual void SetSelfCollisionEnabled(bool bEnabled) = 0;

		/**
		 * @return true if Particle/RigidBody collision is enabled.
		 */
		virtual bool GetPhysXSceneCollisionEnabled() const = 0;

		/**
		 * @param bEnabled - If true, Particle/RigidBody collision is enabled. Affects performance.
		 */
		virtual void SetPhysXSceneCollisionEnabled(bool bEnabled) = 0;

		virtual void SetCollisionQueryInterface(FCollisionQueryInterface* QueryInterface) = 0;

		/**
		 * @return true if performing extra Collision Resolution iterations and an extra constraint iteration to improve collision results.
		 */
		virtual bool GetPerformExtraCollisionResolutionLoop() const = 0;

		/**
		 * @param bEnabled - If true, performs extra Collision Resolution iterations and an extra constraint iteration to improve collision results. NOTE: May impact performance in heavy scenes.
		 */
		virtual void SetPerformExtraCollisionResolutionLoop(bool bEnabled) = 0;


		/**
		 * @return A new SimulatedObject instance. Used to associate and group Particles/Constraints with external Objects such as UE4 Actors/Components.
		 */
		virtual FSimulatedObject* CreateSimulatedObject() = 0;


		/**
		 * Creates and adds a new Particle to the simulation.
		 *
		 * @param IsFree - If true, the particle is free to be simulated. Otherwise it is fixed to its Location.
		 * @param Radius - The desired Particle collision Radius. Also used during Drag simulation.
		 * @param Location - The initial location.
		 * @param Mass - Mass of the Particle.
		 * @param Owner - If not null, associates the created Particle with the supplied SimulatedObject instance.
		 *
		 * @return Instance of the created Particle. NOTE: The created Particles lifetime is managed by the simulation.
		 */
		virtual FSimulatedParticle* CreateParticle(bool IsFree, float Radius, const FVector3Data& Location, float Mass, FSimulatedObject* Owner = nullptr) = 0;

		/**
		 * Removes and destroys the specified Particle from the simulation. Also destroys any Constraints that this Particle is part of.
		 *
		 * @param ParticleIndex - The index of the Particle to destroy.
		 * @param Owner - If not null, updates the SimulatedObject instance.
		 */
		virtual void DestroyParticle(uint_fast32_t ParticleIndex, FSimulatedObject* Owner = nullptr) = 0;

		/**
		 * Creates a new spring Constraint between two Particles and adds it to the simulation.
		 *
		 * @param Particle1Index - Unique index of the first Particle.
		 * @param Particle2Index - Unique index of the second Particle.
		 * @param RestDistance - The desired rest distance. Valid range: 0 to X.
		 * @param Stiffness - The desired stiffness of this constraint. Valid range: 0 to 1.
		 * @param Owner - If not null, associates the created Constraint with the supplied SimulatedObject instance.
		 *
		 * @return Instance of the created Constraint. NOTE: The created Constraints lifetime is managed by the simulation.
		 */
		virtual FParticleSpringConstraint* CreateSpringConstraint(uint_fast32_t Particle1Index, uint_fast32_t Particle2Index, float RestDistance, float Stiffness, FSimulatedObject* Owner = nullptr) = 0;

		/**
		 * Creates a new spring Constraint between two Particles and adds it to the simulation.
		 *
		 * @param Particle1 - First Particle to bind.
		 * @param Particle2 - Second Particle to bind.
		 * @param RestDistance - The desired rest distance. Valid range: 0 to X.
		 * @param Stiffness - The desired stiffness of this constraint. Valid range: 0 to 1.
		 * @param Owner - If not null, associates the created Constraint with the supplied SimulatedObject instance.
		 *
		 * @return Instance of the created Constraint. NOTE: The created Constraints lifetime is managed by the simulation.
		 */
		virtual FParticleSpringConstraint* CreateSpringConstraint(FSimulatedParticle* Particle1, FSimulatedParticle* Particle2, float RestDistance, float Stiffness, FSimulatedObject* Owner = nullptr) = 0;

		/**
		 * Removes and destroys the specified Constraint from the simulation.
		 *
		 * @param ConstraintIndex - The index of the Constraint to destroy.
		 * @param Owner - If not null, updates the SimulatedObject instance.
		 */
		virtual void DestroyConstraint(uint_fast32_t ConstraintIndex, FSimulatedObject* Owner = nullptr) = 0;

		
		virtual void AddExternalConstraint(FExternalConstraint* ExternalConstraint) = 0;
		virtual void RemoveExternalConstraint(FExternalConstraint* ExternalConstraint) = 0;

		/**		 
		 * @return The number of Particles in the Simulation.
		 */
		virtual size_t GetParticleCount() const = 0;

		/**
		 * @return The number of Constraints in the Simulation.
		 */
		virtual size_t GetConstraintCount() const = 0;

		/**
		 * @return The Simulation statistics for profiling. Not available in Release build, returns nullptr.
		 */
		virtual FSimulationStats const* GetSimulationStats() const = 0;


		/**
		 * Sets the PhysX Scenes to use for Rigid Body collision.
		 * 
		 * @param PhysXScene - Used for collision against RigidBodies in that scene. RigidBody collision is disabled if null.
		 * @param PhysXAsyncScene - Used for collision against RigidBodies in that scene. UE4 specific in most cases.
		 */
		virtual void SetPhysXScenes(physx::PxScene* PhysXScene, physx::PxScene* PhysXAsyncScene = nullptr) = 0;



		virtual bool QueryOverlaps(const FQueryShape* QueryShape, const FVector3Data& Location, FQueryOverlapBuffer& OverlapBuffer, const FQueryFilter* Filter = nullptr) = 0;
	};
}

/**
 * Creates a Simulation instance.
 *
 * @param ReserveParticleCount - The max number of simulated particles to allocate memory for. Default is RESERVE_PARTICLE_COUNT = 1024
 * @param BroadphaseMaxParticlesPerCell - The Max number of Particles a cell, in the Broad-phase Spatial Hash, can hold. Make this as low as you can without seeing Particle-Particle collision glitches. Default is DEFAULT_BROADPHASE_MAX_PARTICLES_PER_CELL = 200
 * @param NumThreads - The number of threads to reserve in the Thread Pool, 16 is the max allowed. Only used in *_Parallel builds.
 * @param ThreadAffinities - The affinity each Thread will have. Only used in Xbox One *_Parallel builds.
 *
 * @return The Simulation instance pointer. Use Release to destroy.
 */
extern "C" VICODYNAMICS_API vicoDynamics::FSimulation* VD_CALL_CONV CreateSimulation(int_fast32_t ReserveParticleCount = RESERVE_PARTICLE_COUNT, uint_fast32_t BroadphaseMaxParticlesPerCell = DEFAULT_BROADPHASE_MAX_PARTICLES_PER_CELL, int_fast32_t NumThreads = 4, const uint64_t* ThreadAffinities = nullptr);