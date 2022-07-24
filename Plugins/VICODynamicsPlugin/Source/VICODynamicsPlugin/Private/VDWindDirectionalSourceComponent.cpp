// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDWindDirectionalSourceComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"
#include "TimerManager.h"

UVDWindDirectionalSourceComponent::UVDWindDirectionalSourceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	Radius = 0;
	Strength = 0.1f;
	Speed = 0.1f;
	MinGustAmount = 0.1f;
	MaxGustAmount = 0.2f;
	MinGustDuration = 1.f;
	MaxGustDuration = 5.f;
	MinSecondsBetweenGusts = 2.f;
	MaxSecondsBetweenGusts = 10.f;

	CurrentGust = 0.f;
	TargetGust = 0.f;
	CurrentGustTransitionSpeed = 0.f;
	CurrentGustDurationLeft = 0.f;
}

void UVDWindDirectionalSourceComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IVICODynamicsPlugin::IsAvailable())
	{
		auto VDSimulation = IVICODynamicsPlugin::Get().VDSimulation;
		if (VDSimulation)
		{
			VDSimulation->AddWindSource(this);
			RadiusSquared = Radius * Radius;
		}
	}
}

void UVDWindDirectionalSourceComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentGustDurationLeft <= 0.f && !GetWorld()->GetTimerManager().IsTimerActive(GustTimeoutTimerHandle)) // Timeout
	{
		// Return to no Gust
		TargetGust = 0.f;
		CurrentGustTransitionSpeed = FMath::RandRange(MinGustTransitionSpeed, MaxGustTransitionSpeed);

		const auto SecondsBetweenGusts = FMath::RandRange(MinSecondsBetweenGusts, MaxSecondsBetweenGusts);
		GetWorld()->GetTimerManager().SetTimer(GustTimeoutTimerHandle, [=]() 
		{
			// Setup new Gust parameters
			CurrentGustDurationLeft = FMath::RandRange(MinGustDuration, MaxGustDuration);
			TargetGust = FMath::RandRange(MinGustAmount, MaxGustAmount);
			CurrentGustTransitionSpeed = FMath::RandRange(MinGustTransitionSpeed, MaxGustTransitionSpeed);
		}, SecondsBetweenGusts, false);
	}
	else
	{
		CurrentGust = FMath::FInterpTo(CurrentGust, TargetGust, DeltaTime, CurrentGustTransitionSpeed);
		CurrentGustDurationLeft -= DeltaTime;
	}
}

void UVDWindDirectionalSourceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IVICODynamicsPlugin::IsAvailable())
	{
		auto VDSimulation = IVICODynamicsPlugin::Get().VDSimulation;
		if (VDSimulation)
			VDSimulation->RemoveWindSource(this);
	}

	Super::EndPlay(EndPlayReason);
}

float UVDWindDirectionalSourceComponent::GetCurrentGust() const
{
	return CurrentGust;
}

void UVDWindDirectionalSourceComponent::SetRadius(float InRadius)
{
	Radius = InRadius;
	RadiusSquared = Radius * Radius;
}
