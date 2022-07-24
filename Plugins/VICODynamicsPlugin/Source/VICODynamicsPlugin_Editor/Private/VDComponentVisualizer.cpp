// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDComponentVisualizer.h"
#include "SceneManagement.h"
#include "CanvasTypes.h"
#include "CanvasItem.h"
#include "Engine/Engine.h"
#include "BaseVDComponent.h"
#include "VDRopeComponent.h"
#include "VDDynamicRopeComponent.h"
#include "VDSplineRopeComponent.h"
#include "VDSkinnedRopeComponent.h"
#include "VDProceduralClothComponent.h"
#include "VDMeshClothComponent.h"


template<typename TVDComp>
void DrawConstraints(const TVDComp* VDComp, FPrimitiveDrawInterface* PDI)
{
	static const auto StretchColor = FLinearColor::Blue;
	static const auto BendColor = FLinearColor(.58f, 0.f, .88f);

	const auto& StretchConstraints = VDComp->GetStretchConstraintsArray();
	const auto& BendConstraints = VDComp->GetBendConstraintsArray();

	for (int32 i = 0; i < StretchConstraints.Num(); i++)
	{
		auto Constraint = StretchConstraints[i];
		PDI->DrawLine(Constraint->GetParticle1()->GetLocation(), Constraint->GetParticle2()->GetLocation(), StretchColor, SDPG_Foreground);
	}

	for (int32 i = 0; i < BendConstraints.Num(); i++)
	{
		auto Constraint = BendConstraints[i];
		PDI->DrawLine(Constraint->GetParticle1()->GetLocation(), Constraint->GetParticle2()->GetLocation(), BendColor, SDPG_Foreground);
	}
}

void FVDComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	auto VDComp = Cast<UBaseVDComponent>(Component);

	if (!VDComp) return;

	if (VDComp->bVisualizeConstraints)
	{
		if (auto RopeComp = Cast<UVDRopeComponent>(VDComp))
		{
			DrawConstraints(RopeComp, PDI);
		}
		else if (auto MeshClothComp = Cast<UVDMeshClothComponent>(VDComp))
		{
			DrawConstraints(MeshClothComp, PDI);
		}
		else if (auto ProcClothComp = Cast<UVDProceduralClothComponent>(VDComp))
		{
			DrawConstraints(ProcClothComp, PDI);
		}
	}
}

void FVDComponentVisualizer::DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	auto VDComp = Cast<UBaseVDComponent>(Component);

	if (!VDComp) return;

	auto ParticleInfo = VDComp->GetParticleBuildInfo();
	if (ParticleInfo && VDComp->bShowParticleIndices)
	{
		for (int32 i = 0; i < ParticleInfo->Num(); i++)
		{
			const FVDParticleInfo& Info = (*ParticleInfo)[i];
			DrawString(FString::Printf(TEXT("%d"), i), VDComp->GetUseCachedLocations() ? VDComp->GetComponentTransform().TransformPosition(VDComp->GetCachedLocations()[i].Location) : Info.Location, View, Canvas);
		}
	}
}

FVector ProjectToScreenSpace(FVector WorldLocation, const FSceneView* View, const FCanvas* Canvas)
{
	const auto ViewPlane = View->Project(WorldLocation);
	FVector ScreenLoc = ViewPlane;

	const auto& ViewRect = Canvas->GetViewRect();
	const auto Width = ViewRect.Width();
	const auto Height = ViewRect.Height();
	ScreenLoc.X = (Width * .5f) + (ScreenLoc.X * (Width * .5f));
	ScreenLoc.Y *= -1.f * GProjectionSignY;
	ScreenLoc.Y = (Height * .5f) + (ScreenLoc.Y * (Height * .5f));

	// if behind the screen, clamp depth to the screen
	if (ViewPlane.W <= 0.0f)
	{
		ScreenLoc.Z = 0.0f;
	}

	return ScreenLoc;
}

void FVDComponentVisualizer::DrawString(FString String, FVector Location, const FSceneView* View, FCanvas* Canvas)
{
	FRotator CameraRot = View->ViewRotation;
	FVector CameraLoc = View->ViewLocation;

	FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), GEngine->GetSmallFont(), FLinearColor::White);
	TextItem.Font = GEngine->GetSmallFont();

	//TextItem.EnableShadow(FLinearColor::Black);

	// don't draw text behind the camera
	if (((Location - CameraLoc) | CameraRot.Vector()) > 0.f)
	{
		const auto ScreenLoc = ProjectToScreenSpace(Location, View, Canvas); 

		TextItem.SetColor(FColor::White);
		TextItem.Text = FText::FromString(String);
		TextItem.Scale = FVector2D(1.f, 1.f);
		Canvas->DrawItem(TextItem, FVector2D(FMath::CeilToFloat(ScreenLoc.X), FMath::CeilToFloat(ScreenLoc.Y)));
	}
}