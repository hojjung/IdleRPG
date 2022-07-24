// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "GameFramework/Info.h"
#include "VDWindDirectionalSourceComponent.h"
#include "Components/ArrowComponent.h"
#include "VDWindDirectionalSourceActor.generated.h"

/** Actor that provides a directional wind source. Affects VICODynamics Objects and SpeedTree assets. */
UCLASS(ClassGroup = Wind, showcategories = (Rendering, "Utilities|Transformation"))
class VICODYNAMICSPLUGIN_API AVDWindDirectionalSourceActor : public AInfo
{
	GENERATED_BODY()
	AVDWindDirectionalSourceActor(const FObjectInitializer& ObjectInitializer);

public:
	class UVDWindDirectionalSourceComponent* GetComponent() const;

#if WITH_EDITORONLY_DATA
	class UArrowComponent* GetArrowComponent() const;
#endif

private:
		UPROPERTY(Category = WindDirectionalSource, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UVDWindDirectionalSourceComponent* Component;

#if WITH_EDITORONLY_DATA
		UPROPERTY()
		class UArrowComponent* ArrowComponent;
#endif
};