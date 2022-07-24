// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDSimulatedObject.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "Simulation.h"
#include "SimulatedObject.h"
#include "SimulatedParticle.h"
#include "IVICODynamicsPlugin.h"

#include "Collision.h"

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#	if WITH_EDITORONLY_DATA
#	include "Editor.h"
#	endif

#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#endif

#define LOCTEXT_NAMESPACE "UVDSimulatedObject"

UVDSimulatedObject::UVDSimulatedObject(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SimulatedObject = nullptr;

	CollisionSettings.CollisionEnabled = ECollisionEnabled::QueryAndPhysics;
	CollisionSettings.CollisionResponse.SetAllChannels(ECR_Block);
	CollisionSettings.ObjectType = ECollisionChannel::ECC_PhysicsBody;

	GravityScale = 1.f;
	MinConstraintDistance = .007f;
}

void UVDSimulatedObject::BeginDestroy()
{
	Super::BeginDestroy();
}

void UVDSimulatedObject::ReleaseNow()
{
	if (SimulatedObject && !IVICODynamicsPlugin::Get().Released)
	{
		SimulatedObject->Release();
		SimulatedObject = nullptr;

		if (IVICODynamicsPlugin::Get().VDSimulation)
			IVICODynamicsPlugin::Get().VDSimulation->UpdateSimStats();
	}
}

void UVDSimulatedObject::CheckInit()
{
	if (SimulatedObject) return;

	SimulatedObject = IVICODynamicsPlugin::Get().VDSimulation->GetSimulation()->CreateSimulatedObject();
	SimulatedObject->UserData = this;

	SetCollisionChannelAndResponses(CollisionSettings.ObjectType, CollisionSettings.CollisionResponse);
	SetCollisionEnabled(CollisionSettings.CollisionEnabled != ECollisionEnabled::NoCollision);

	SetGravityScale(GravityScale);
	SetMinimumConstraintSatisfactionDistance(MinConstraintDistance);
}

UVDSimulatedParticle* UVDSimulatedObject::CreateParticle(const FVDParticleInfo& ParticleInfo)
{
	CheckInit();

	auto Simulation = IVICODynamicsPlugin::Get().VDSimulation->GetSimulation();

	if (!Simulation) return nullptr;

	auto vdParticle = Simulation->CreateParticle(ParticleInfo.IsFree, ParticleInfo.Radius, UVectorToVDVector(ParticleInfo.Location), ParticleInfo.Mass, SimulatedObject);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!vdParticle)
	{
		FMessageLog("PIE").Error()
			->AddToken(FUObjectToken::Create(GetOuter()))
			->AddToken(FTextToken::Create(LOCTEXT("CreateParticleStart", ": Calling CreateParticle on")))
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(LOCTEXT("CreateParticleEnd", "but failed to create Particle, make sure you are not creating more Partcles than memory reserved for in Project Setting->VICODynamics->Reserve Particle Budget! Increase that setting if needed.")));

#if WITH_EDITORONLY_DATA
		GEditor->RequestEndPlayMap();
#endif
		FMessageLog("PIE").Open(EMessageSeverity::Error);
		return nullptr;
	}
#else
	check(vdParticle);
#endif
	vdParticle->SetDragCoefficient(ParticleInfo.DragCoefficient);

	auto particle = NewObject<UVDSimulatedParticle>(this);
	particle->SetUnderlyingParticle(vdParticle);
	
	return particle;
}

UVDParticleSpringConstraint* UVDSimulatedObject::CreateSpringConstraint(UVDSimulatedParticle* Particle1, UVDSimulatedParticle* Particle2, float RestDistance, float Stiffness)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	bool bCancelCreation = false;
	if (!Particle1)
	{
		FMessageLog("PIE").Error()
			->AddToken(FUObjectToken::Create(GetOuter()))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP1Start", ": Calling CreateSpringConstraint on")))
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP1End", "but failed to create Constraint becuase Particle1 is null")));
		bCancelCreation = true;
	} 
	else if (!Particle1->GetUnderlyingParticle())
	{
		FMessageLog("PIE").Error()
			->AddToken(FUObjectToken::Create(GetOuter()))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP1PStart", ": Calling CreateSpringConstraint on")))
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP1PMid", "but failed to create Constraint becuase Particle1's,")))
			->AddToken(FUObjectToken::Create(Particle1))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP1PEnd", ", underlying Particle pointer is null")));
		bCancelCreation = true;
	}

	if (!Particle2)
	{
		FMessageLog("PIE").Error()
			->AddToken(FUObjectToken::Create(GetOuter()))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP2Start", ": Calling CreateSpringConstraint on")))
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP2End", "but failed to create Constraint becuase Particle2 is null")));
		bCancelCreation = true;
	}
	else if (!Particle2->GetUnderlyingParticle())
	{
		FMessageLog("PIE").Error()
			->AddToken(FUObjectToken::Create(GetOuter()))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP2PStart", ": Calling CreateSpringConstraint on")))
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP2PMid", "but failed to create Constraint becuase Particle2's,")))
			->AddToken(FUObjectToken::Create(Particle2))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintP2PEnd", ", underlying Particle pointer is null")));
		bCancelCreation = true;
	}

	if (Particle1->GetUnderlyingParticle() == Particle2->GetUnderlyingParticle())
	{
		FMessageLog("PIE").Error()
			->AddToken(FUObjectToken::Create(GetOuter()))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintPCompareStart", ": Calling CreateSpringConstraint on")))
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(LOCTEXT("CreateConstraintPCompareEnd", "but failed to create Constraint becuase Particle1 and Particle2 are referencing the same underling VDSimulatedParticle")));
		bCancelCreation = true;
	}

	if (bCancelCreation)
	{
#if WITH_EDITORONLY_DATA
		GEditor->RequestEndPlayMap();
#endif
		FMessageLog("PIE").Open(EMessageSeverity::Error);
		return nullptr;
	}
#else
	check(Particle1);
	check(Particle1->GetUnderlyingParticle());
	check(Particle2);
	check(Particle2->GetUnderlyingParticle());
	check(Particle1->GetUnderlyingParticle() != Particle2->GetUnderlyingParticle());
#endif
	CheckInit();

	auto Simulation = IVICODynamicsPlugin::Get().VDSimulation->GetSimulation();

	if (!Simulation) return nullptr;

	auto vdSpring = Simulation->CreateSpringConstraint(Particle1->GetUnderlyingParticle(), Particle2->GetUnderlyingParticle(), RestDistance, Stiffness, SimulatedObject);

#if WITH_EDITORONLY_DATA
	if (!vdSpring)
	{
		UE_LOG(LogVICODynamics, Error, TEXT("Failed to create Constraint, make sure you are not creating more Constraints than memory reserved for in Project Setting->VICODynamics->Reserve Particle Budget! \nIncrease that setting if needed. NOTE: Constraint budget = 8 * Particle Budget."));
		GEditor->RequestEndPlayMap();
		return nullptr;
	}
#else
	check(vdSpring);
#endif

	auto spring = NewObject<UVDParticleSpringConstraint>(this);
	spring->SetUnderlyingConstraint(vdSpring);
	
	return spring;
}

void UVDSimulatedObject::DestroyParticle(UVDSimulatedParticle* Particle)
{
	CheckInit();
	check(SimulatedObject);
	check(Particle);

	auto Simulation = IVICODynamicsPlugin::Get().VDSimulation->GetSimulation();

	if (!Simulation || !Particle->GetUnderlyingParticle()->IsValid()) return;

	Simulation->DestroyParticle(Particle->GetUnderlyingParticle()->GetIndex(), SimulatedObject);
	Particle->MarkPendingKill();
}

void UVDSimulatedObject::DestroyConstraint(UVDParticleSpringConstraint* Constraint)
{
	CheckInit();
	check(SimulatedObject);
	check(Constraint);

	auto Simulation = IVICODynamicsPlugin::Get().VDSimulation->GetSimulation();

	if (!Simulation || !Constraint->GetUnderlyingConstraint()->IsValid()) return;

	Simulation->DestroyConstraint(Constraint->GetUnderlyingConstraint()->GetIndex(), SimulatedObject);
	Constraint->MarkPendingKill();
}

TArray<UVDSimulatedParticle*> UVDSimulatedObject::GetParticles()
{
	TArray<UVDSimulatedParticle*> Particles;
	GetParticles(Particles);

	return Particles;
}

int32 UVDSimulatedObject::GetParticles(TArray<UVDSimulatedParticle*>& OutParticles)
{
	CheckInit();
	check(SimulatedObject);

	const auto& particleCount = SimulatedObject->GetParticleCount();

	TArray<vicoDynamics::FSimulatedParticle*> particles;
	particles.AddDefaulted(particleCount);

	const auto& particlesReceived = SimulatedObject->GetParticles(&particles[0], particleCount);

	if (particlesReceived == 0) return 0;

	OutParticles.Reset(particlesReceived);

	for (size_t i = 0; i < particlesReceived; i++)
	{
		if (particles[i]->IsValid())
		{
			auto userData = particles[i]->UserData;
			if (userData)
			{
				auto objPtr = static_cast<TWeakObjectPtr<UVDSimulatedParticle>*>(userData);
				if (objPtr)
				{
					auto particle = objPtr->Get();
					if (particle)
						OutParticles.Add(particle);
				}
			}
		}
	}

	return OutParticles.Num();
}

TArray<UVDParticleSpringConstraint*> UVDSimulatedObject::GetSpringConstraints()
{
	TArray<UVDParticleSpringConstraint*> SpringConstraints;
	GetSpringConstraints(SpringConstraints);

	return SpringConstraints;
}

int32 UVDSimulatedObject::GetSpringConstraints(TArray<UVDParticleSpringConstraint*>& OutSpringConstraints)
{
	CheckInit();
	check(SimulatedObject);

	const auto& constraintCount = SimulatedObject->GetConstraintCount();

	TArray<vicoDynamics::FParticleSpringConstraint*> constraints;
	constraints.AddDefaulted(constraintCount);

	const auto& constraintsReceived = SimulatedObject->GetConstraints(&constraints[0], constraintCount);

	if (constraintsReceived == 0) return 0;

	OutSpringConstraints.Reset(constraintsReceived);

	for (size_t i = 0; i < constraintsReceived; i++)
	{
		if (constraints[i]->IsValid())
		{
			auto userData = constraints[i]->UserData;
			if (userData)
			{
				auto objPtr = static_cast<TWeakObjectPtr<UVDParticleSpringConstraint>*>(userData);
				if (objPtr)
				{
					auto constraint = objPtr->Get();
					if (constraint)
						OutSpringConstraints.Add(constraint);
				}
			}
		}
	}

	return OutSpringConstraints.Num();
}

void UVDSimulatedObject::RemoveFromSimulation()
{
	CheckInit();
	check(SimulatedObject);

	SimulatedObject->RemoveFromSimulation();
}

float UVDSimulatedObject::GetGravityScale() const
{
	check(SimulatedObject);

	return SimulatedObject->GetGravityScale();
}

void UVDSimulatedObject::SetGravityScale(float Scale)
{
	CheckInit();
	check(SimulatedObject);

	SimulatedObject->SetGravityScale(Scale);
	GravityScale = Scale;
}

FVector UVDSimulatedObject::GetAffectiveWindVector() const
{
	check(SimulatedObject);

	return VDVectorToUVector(SimulatedObject->GetAffectiveWindVector());
}

void UVDSimulatedObject::SetAffectiveWindVector(FVector NewWindVector)
{
	CheckInit();
	check(SimulatedObject);

	SimulatedObject->SetAffectiveWindVector(UVectorToVDVector(NewWindVector));
}

bool UVDSimulatedObject::GetCollisionEnabled() const
{
	check(SimulatedObject);

	return SimulatedObject->GetCollisionEnabled();
}

void UVDSimulatedObject::SetCollisionEnabled(bool Enabled)
{
	CheckInit();
	check(SimulatedObject);

	SimulatedObject->SetCollisionEnabled(Enabled);
}

float UVDSimulatedObject::GetMinimumConstraintSatisfactionDistance() const
{
	check(SimulatedObject);

	return SimulatedObject->GetMinimumConstraintSatisfactionDistance();
}

void UVDSimulatedObject::SetMinimumConstraintSatisfactionDistance(float MinDistance)
{
	CheckInit();
	check(SimulatedObject);

	SimulatedObject->SetMinimumConstraintSatisfactionDistance(MinDistance);
	MinConstraintDistance = MinDistance;
}

bool UVDSimulatedObject::GetSkipRelativeVelocityCheckDuringCollision() const
{
	check(SimulatedObject);

	return SimulatedObject->GetSkipRelativeVelocityCheckDuringCollision();
}

void UVDSimulatedObject::SetSkipRelativeVelocityCheckDuringCollision(bool Skip)
{
	CheckInit();
	check(SimulatedObject);

	SimulatedObject->SetSkipRelativeVelocityCheckDuringCollision(Skip);
}

FVDCollisionSettings UVDSimulatedObject::GetCollisionSettings() const
{
	return CollisionSettings;
}

void UVDSimulatedObject::SetCollisionSettings(const FVDCollisionSettings& Settings)
{
	CollisionSettings = Settings;

	if (SimulatedObject)
	{
		//SetCollisionChannelAndResponses(CollisionSettings.ObjectType, CollisionSettings.CollisionResponse);
		SetCollisionEnabled(CollisionSettings.CollisionEnabled != ECollisionEnabled::NoCollision);
	}
}

///////////////////////DEPRECATED!/////////////////////////////////////////////////////////

/*
 * Copied from PhysXCollision.cpp since they are not exported by the engine.
 */

/**
* Set of flags stored in the PhysX FilterData
*
* When this flag is saved in CreateShapeFilterData or CreateQueryFilterData, we only use 24 bits
* If you plan to use more than 24 bits, you'll also need to change the format of ShapeFilterData,QueryFilterData
* Make sure you also change preFilter/SimFilterShader where it's used
*/
 
// namespace EPhysXFilterDataFlags_VD
// {
// 	enum Type
// 	{
// 		EPDF_SimpleCollision = 0x0001,
// 		EPDF_ComplexCollision = 0x0002,
// 		EPDF_CCD = 0x0004,
// 		EPDF_ContactNotify = 0x0008,
// 		EPDF_StaticShape = 0x0010,
// 		EPDF_ModifyContacts = 0x0020
// 	};
// }

/**
* Type of query for object type or trace type
* Trace queries correspond to trace functions with TravelChannel/ResponseParams
* Object queries correspond to trace functions with Object types
*/
// namespace ECollisionQuery
// {
// 	enum Type
// 	{
// 		ObjectQuery = 0,
// 		TraceQuery = 1
// 	};
// }
// 
// #define TRACE_MULTI		1
// #define TRACE_SINGLE	0
// 
// uint32 CreateChannelAndFilter_VD(ECollisionChannel CollisionChannel, FMaskFilter MaskFilter)
// {
// 	uint32 ResultMask = (MaskFilter << 5) | (uint32)CollisionChannel;
// 	return ResultMask << 23;
// }
// 
// PxFilterData CreateObjectQueryFilterData_VD(const bool bTraceComplex, const int32 MultiTrace/*=1 if multi. 0 otherwise*/, const struct FCollisionObjectQueryParams & ObjectParam)
// {
// 	/**
// 	* Format for QueryData :
// 	*		word0 (meta data - ECollisionQuery. Extendable)
// 	*
// 	*		For object queries
// 	*
// 	*		word1 (object type queries)
// 	*		word2 (unused)
// 	*		word3 (Multi (1) or single (0) (top 8) + Flags (lower 24))
// 	*/
// 
// 	PxFilterData PNewData;
// 
// 	PNewData.word0 = ECollisionQuery::ObjectQuery;
// 
// 	if (bTraceComplex)
// 	{
// 		PNewData.word3 |= EPDF_ComplexCollision;
// 	}
// 	else
// 	{
// 		PNewData.word3 |= EPDF_SimpleCollision;
// 	}
// 
// 	// get object param bits
// 	PNewData.word1 = ObjectParam.GetQueryBitfield();
// 
// 	// if 'nothing', then set no bits
// 	PNewData.word3 |= CreateChannelAndFilter_VD((ECollisionChannel)MultiTrace, ObjectParam.IgnoreMask);
// 
// 	return PNewData;
// }
// 
// PxFilterData CreateTraceQueryFilterData_VD(const uint8 MyChannel, const bool bTraceComplex, const FCollisionResponseContainer& InCollisionResponseContainer)
// {
// 	/**
// 	* Format for QueryData :
// 	*		word0 (meta data - ECollisionQuery. Extendable)
// 	*
// 	*		For trace queries
// 	*
// 	*		word1 (blocking channels)
// 	*		word2 (touching channels)
// 	*		word3 (MyChannel (top 8) as ECollisionChannel + Flags (lower 24))
// 	*/
// 
// 	PxFilterData PNewData;
// 
// 	PNewData.word0 = ECollisionQuery::TraceQuery;
// 
// 	if (bTraceComplex)
// 	{
// 		PNewData.word3 |= EPDF_ComplexCollision;
// 	}
// 	else
// 	{
// 		PNewData.word3 |= EPDF_SimpleCollision;
// 	}
// 
// 	// word1 encodes 'what i block', word2 encodes 'what i touch'
// 	for (int32 i = 0; i<ARRAY_COUNT(InCollisionResponseContainer.EnumArray); i++)
// 	{
// 		if (InCollisionResponseContainer.EnumArray[i] == ECR_Block)
// 		{
// 			// if i block, set that in word1
// 			PNewData.word1 |= CRC_TO_BITFIELD(i);
// 		}
// 		else if (InCollisionResponseContainer.EnumArray[i] == ECR_Overlap)
// 		{
// 			// if i touch, set that in word2
// 			PNewData.word2 |= CRC_TO_BITFIELD(i);
// 		}
// 	}
// 
// 	// if 'nothing', then set no bits
// 	PNewData.word3 |= CreateChannelAndFilter_VD((ECollisionChannel)MyChannel, FCollisionQueryParams::DefaultQueryParam.IgnoreMask);
// 
// 	return PNewData;
// }
// 
// /** Utility for creating a PhysX PxFilterData for performing a query (trace) against the scene */
// PxFilterData CreateQueryFilterData_VD(const uint8 MyChannel, const bool bTraceComplex, const FCollisionResponseContainer& InCollisionResponseContainer, const struct FCollisionObjectQueryParams & ObjectParam, const bool bMultitrace)
// {
// 	if (ObjectParam.IsValid())
// 	{
// 		return CreateObjectQueryFilterData_VD(bTraceComplex, (bMultitrace ? TRACE_MULTI : TRACE_SINGLE), ObjectParam);
// 	}
// 	else
// 	{
// 		return CreateTraceQueryFilterData_VD(MyChannel, bTraceComplex, InCollisionResponseContainer);
// 	}
// }

////////////////////////////////////////////////////////////////////////////////

void UVDSimulatedObject::SetCollisionChannelAndResponses(enum ECollisionChannel Channel, struct FCollisionResponse Responses)
{
	// DEPRECATED
// 	static_assert(sizeof(vicoDynamics::FCollisionFilter) == sizeof(PxFilterData), "FCollisionFilter is not compatible with PxFilterData!");
// 
// 	auto pFilterData = CreateQueryFilterData_VD(Channel, false, Responses.GetResponseContainer(), FCollisionObjectQueryParams::DefaultObjectQueryParam, false);
// 	SimulatedObject->CollisionFilter = *reinterpret_cast<vicoDynamics::FCollisionFilter*>(&pFilterData); // Assuming FCollisionFilter is laid out for PhysX, it is by default. If not, will be caught by above check.

	CollisionSettings.ObjectType = Channel;
	CollisionSettings.CollisionResponse = Responses;
}

void UVDSimulatedObject::SetCollisionChannel(enum ECollisionChannel Channel)
{
	CollisionSettings.ObjectType = Channel;
}

void UVDSimulatedObject::SetObjectType(enum ECollisionChannel ObjectType)
{
	CollisionSettings.ObjectType = ObjectType;
}

void UVDSimulatedObject::SetCollisionResponseToObjectType(enum ECollisionChannel ObjectType, enum ECollisionResponse Response)
{
	CollisionSettings.CollisionResponse.SetResponse(ObjectType, Response);
}

#undef LOCTEXT_NAMESPACE