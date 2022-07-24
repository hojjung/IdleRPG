// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

class FPrimitiveDrawInterface;
class FSceneView;

class VICODYNAMICSPLUGIN_EDITOR_API FVDComponentVisualizer : public FComponentVisualizer
{
public:
	// Begin FComponentVisualizer Interface
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	// End FComponentVisualizer Interface

protected:
	void DrawString(FString String, FVector Location, const FSceneView* View, FCanvas* Canvas);
};