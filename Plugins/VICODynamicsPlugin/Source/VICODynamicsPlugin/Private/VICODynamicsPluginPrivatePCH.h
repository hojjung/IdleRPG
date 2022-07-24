// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "CoreMinimal.h"
//#include "Engine.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "IVICODynamicsPlugin.h"

#include "Simulation.h"
#include "SimulatedObject.h"

#include "VDSimulation.h"
#include "VDSimulatedObject.h"
#include "VDSimulatedParticle.h"

#ifdef WITH_APEX
#	if WITH_APEX
#		define REDEFINE_WITH_APEX
#	endif
#	undef WITH_APEX
#	define WITH_APEX 0
#endif // WITH_APEX

#include "PhysXIncludes.h"
#include "PhysXPublic.h"
#include "Runtime/Engine/Private/PhysicsEngine/PhysXSupport.h"

#ifdef REDEFINE_WITH_APEX
#   undef WITH_APEX
#	define WITH_APEX (1 && WITH_PHYSX)
#	undef REDEFINE_WITH_APEX
#endif // REDEFINE_WITH_APEX