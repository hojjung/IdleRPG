// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VICODynamicsSettings.h"
#include "VICODynamicsPluginPrivatePCH.h"

UVICODynamicsSettings::UVICODynamicsSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReserveParticleBudget = RESERVE_PARTICLE_COUNT;
	NumberOfThreadsToUse = 4;
	EnableSelfCollision = true;
	EnablePhysXSceneCollision = true;
	PerformExtraCollisionResolutionLoop = false;
	EnableExplicitRigidBodyCollision = false;
	SimulationFramerate = 60;
	RunSimulationOnWorkerThread = false;
	WorkerThreadCoreAffinity = 1;
	SimulationIterations = 3;
	ConstraintIterations = 8;
	ConstrainSimulationTo2DPlane = false;
	PlaneOrigin = FVector::ZeroVector;
	PlaneNormal = FVector::RightVector;
	bCachePerFrameResults = false;
	bCacheOnlyCollisionsWithStaticRigidBodies = false;
	bCacheNonHitsAlso = true;
	DrawCollisionDebug = false;
	bShowOnScreenStats = false;
}

FVDSettings::FVDSettings()
{
	ReserveParticleBudget = RESERVE_PARTICLE_COUNT;
	NumberOfThreadsToUse = 4;
	EnableSelfCollision = true;
	EnablePhysXSceneCollision = true;
	PerformExtraCollisionResolutionLoop = false;
	EnableExplicitRigidBodyCollision = false;
	SimulationFramerate = 60;
	RunSimulationOnWorkerThread = false;
	WorkerThreadCoreAffinity = 1;
	SimulationIterations = 3;
	ConstraintIterations = 8;
	ConstrainSimulationTo2DPlane = false;
	PlaneOrigin = FVector::ZeroVector;
	PlaneNormal = FVector::RightVector;
	bCachePerFrameResults = false;
	bCacheOnlyCollisionsWithStaticRigidBodies = false;
	bCacheNonHitsAlso = true;
	DrawCollisionDebug = false;
	bShowOnScreenStats = false;
}

void FVDSettings::CopyFrom(const UVICODynamicsSettings* ConfigSettings)
{
	if (!bOverride_ReserveParticleBudget)
		ReserveParticleBudget = ConfigSettings->ReserveParticleBudget;
	
	if (!bOverride_NumberOfThreadsToUse)
		NumberOfThreadsToUse = ConfigSettings->NumberOfThreadsToUse;
	
	if (!bOverride_EnableSelfCollision)
		EnableSelfCollision = ConfigSettings->EnableSelfCollision;
	
	if (!bOverride_EnablePhysXSceneCollision)
		EnablePhysXSceneCollision = ConfigSettings->EnablePhysXSceneCollision;

	if (!bOverride_PerformExtraCollisionResolutionLoop)
		PerformExtraCollisionResolutionLoop = ConfigSettings->PerformExtraCollisionResolutionLoop;
	
	if (!bOverride_EnableExplicitRigidBodyCollision)
		EnableExplicitRigidBodyCollision = ConfigSettings->EnableExplicitRigidBodyCollision;
	
	if (!bOverride_SimulationFramerate)
		SimulationFramerate = ConfigSettings->SimulationFramerate;
	
	if (!bOverride_RunSimulationOnWorkerThread)
		RunSimulationOnWorkerThread = ConfigSettings->RunSimulationOnWorkerThread;
	
	if (!bOverride_WorkerThreadCoreAffinity)
		WorkerThreadCoreAffinity = ConfigSettings->WorkerThreadCoreAffinity;
	
	if (!bOverride_SimulationIterations)
		SimulationIterations = ConfigSettings->SimulationIterations;
	
	if (!bOverride_ConstraintIterations)
		ConstraintIterations = ConfigSettings->ConstraintIterations;
	
	if (!bOverride_ConstrainSimulationTo2DPlane)
		ConstrainSimulationTo2DPlane = ConfigSettings->ConstrainSimulationTo2DPlane;

	if (!bOverride_PlaneOrigin)
		PlaneOrigin = ConfigSettings->PlaneOrigin;

	if (!bOverride_PlaneNormal)
		PlaneNormal = ConfigSettings->PlaneNormal;

	if (!bOverride_CachePerFrameResults)
		bCachePerFrameResults = ConfigSettings->bCachePerFrameResults;
	
	if (!bOverride_CacheOnlyCollisionsWithStaticRigidBodies)
		bCacheOnlyCollisionsWithStaticRigidBodies = ConfigSettings->bCacheOnlyCollisionsWithStaticRigidBodies;

	if (!bOverride_CacheNonHitsAlso)
		bCacheNonHitsAlso = ConfigSettings->bCacheNonHitsAlso;
	
	DrawCollisionDebug = ConfigSettings->DrawCollisionDebug;
	bShowOnScreenStats = ConfigSettings->bShowOnScreenStats;
}