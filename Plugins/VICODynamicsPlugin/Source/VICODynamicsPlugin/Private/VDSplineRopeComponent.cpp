// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDSplineRopeComponent.h"
#include "VICODynamicsPluginPrivatePCH.h"

UVDSplineRopeComponent::UVDSplineRopeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

#if WITH_EDITOR
void UVDSplineRopeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	auto Property = PropertyChangedEvent.Property;

	if (Property)
	{
		if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UVDSplineRopeComponent, SplineRef))
		{
			auto Spline = Cast<USplineComponent>(SplineRef.GetComponent(GetOwner()));
			if (Spline)
				SplineLength = Spline->GetSplineLength();
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

void UVDSplineRopeComponent::RegisterWithSimulation()
{
	if (bAutoAttachToSplineEndPoints && !AttachToRigids)
	{
		auto Spline = Cast<USplineComponent>(SplineRef.GetComponent(GetOwner()));

		if (Spline)
		{
			ManuallyAttachParticles = true; // Make sure the attachment happens.

			FVDParticleAttachment SplineStartAttachment;
			FVDParticleAttachment SplineEndAttachment;

			SplineStartAttachment.Location = Spline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::Local);
			SplineStartAttachment.ParticleIndex = 0;
			SplineStartAttachment.IsRelative = true;

			SplineEndAttachment.Location = Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::Local);
			SplineEndAttachment.ParticleIndex = NumSegments;
			SplineEndAttachment.IsRelative = true;

			const auto StartAttachmentIndex = ParticleAttachments.IndexOfByPredicate([](const FVDParticleAttachment& Attachment) {return Attachment.ParticleIndex == 0;});
			const auto EndAttachmentIndex = ParticleAttachments.IndexOfByPredicate([=](const FVDParticleAttachment& Attachment) {return Attachment.ParticleIndex == NumSegments;});

			if (StartAttachmentIndex != INDEX_NONE)
				ParticleAttachments[StartAttachmentIndex] = SplineStartAttachment;
			else
				ParticleAttachments.Add(SplineStartAttachment);

			if (EndAttachmentIndex != INDEX_NONE)
				ParticleAttachments[EndAttachmentIndex] = SplineEndAttachment;
			else
				ParticleAttachments.Add(SplineEndAttachment);
		}
	}

	Super::RegisterWithSimulation();
}

void UVDSplineRopeComponent::CreateRopeGeometry(TArray<FVDParticleInfo>& RopePoints)
{
	auto Spline = Cast<USplineComponent>(SplineRef.GetComponent(GetOwner()));

	if (Spline)
	{
		SplineLength = Spline->GetSplineLength();
		if (Length)
		{
			RopePoints.Reset();

			const auto OffsetBetweenParticles = Length / NumSegments;
			const int32 NumParticles = NumSegments + 1;
			const float PerParticleMass = Mass / NumParticles;

			for (int32 i = 0; i < NumParticles; i++)
			{
				const FVector InitialPosition = GetComponentLocation() + GetComponentRotation().RotateVector(bStretchToSplineLength ? 
																											Spline->GetLocationAtTime((static_cast<float>(i) / NumParticles) * Spline->Duration, ESplineCoordinateSpace::Local, true) : 
																											Spline->GetLocationAtDistanceAlongSpline(OffsetBetweenParticles * i, ESplineCoordinateSpace::Local));

				FVDParticleInfo particleInfo;
				particleInfo.IsFree = true;
				particleInfo.Radius = ParticleRadius;
				particleInfo.Location = InitialPosition;
				particleInfo.Mass = PerParticleMass;
				particleInfo.DragCoefficient = DragCoefficient;
				RopePoints.Add(particleInfo);
			}

			if (ManuallyAttachParticles)
			{
				// Apply user defined attachments, to visualize the attachment in editor
				for (auto& Attachment : ParticleAttachments)
				{
					if (Attachment.ParticleIndex < 0 || Attachment.ParticleIndex >= NumParticles) continue;

					auto& particleInfo = RopePoints[Attachment.ParticleIndex];
					const auto prevLocation = particleInfo.Location;
					particleInfo.Location = ResolveAttachmentLocation(Attachment);
					particleInfo.IsFree = false;
					Attachment.OldMass = particleInfo.Mass;
					particleInfo.Mass = 0.f;
				}
			}
		}
	}
}