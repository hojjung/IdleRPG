// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDColliderComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "BaseVDComponent.h"
#include "Components/PrimitiveComponent.h"

UVDColliderComponent::UVDColliderComponent(const FObjectInitializer& ObjectInitializer)
{
	bAddAllComponents = true;
	bIgnoreConvexShapes = true;

	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UPrimitiveComponent* FindComponentByPropertyName(AActor* Owner, FName ComponentPropertyName)
{
	if (ComponentPropertyName != NAME_None)
	{
		UObjectPropertyBase* ObjProp = FindField<UObjectPropertyBase>(Owner->GetClass(), ComponentPropertyName);
		if (ObjProp != NULL)
		{
			// .. and return the component that is there
			return Cast<UPrimitiveComponent>(ObjProp->GetObjectPropertyValue_InContainer(Owner));
		}
	}

	return nullptr;
}

void UVDColliderComponent::BeginPlay()
{
	Super::BeginPlay();

	if ((!GIsEditor || GIsPlayInEditorWorld) && IVICODynamicsPlugin::IsAvailable())
	{
		auto VDSimulation = IVICODynamicsPlugin::Get().VDSimulation;
		if (VDSimulation)
		{
			auto Simulation = VDSimulation->GetSimulation();
			check(Simulation);
			SimulationInstance = VDSimulation;

			AActor* Owner = GetOwner();
			for (const auto& VDCompRef : AssociatedVDComponents)
			{
				auto VDComp = Cast<UBaseVDComponent>(VDCompRef.GetComponent(Owner));
				if (VDComp)
				{
					if (bAddAllComponents)
					{
						auto Components = Owner->GetComponentsByClass(UPrimitiveComponent::StaticClass());
						for (int32 i = 0; i < Components.Num(); i++)
							SimulationInstance->AddExplicitColliderMapping(VDComp, Cast<UPrimitiveComponent>(Components[i]), bIgnoreConvexShapes);
					}
					else
					{
						for (int32 i = 0; i < SpecificColliders.Num(); i++)
							if (auto Collider = FindComponentByPropertyName(Owner, SpecificColliders[i]))
								SimulationInstance->AddExplicitColliderMapping(VDComp, Collider, bIgnoreConvexShapes);
					}
				}
			}
		}
	}
}