// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "VDWindDirectionalSourceActor.h"
#include "VICODynamicsPluginPrivatePCH.h"
#include "IVICODynamicsPlugin.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BillboardComponent.h"

AVDWindDirectionalSourceActor::AVDWindDirectionalSourceActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Component = CreateDefaultSubobject<UVDWindDirectionalSourceComponent>(TEXT("VDWindDirectionalSourceComponent0"));
	RootComponent = Component;

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent0"));

	if (!IsRunningCommandlet())
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_Wind;
			FText NAME_Wind;
			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/S_WindDirectional"))
				, ID_Wind(TEXT("Wind"))
				, NAME_Wind(NSLOCTEXT("SpriteCategory", "Wind", "Wind"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		if (ArrowComponent)
		{
			ArrowComponent->ArrowColor = FColor(150, 200, 255);
			ArrowComponent->bTreatAsASprite = true;
			ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Wind;
			ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Wind;
			ArrowComponent->SetupAttachment(Component);
			ArrowComponent->bIsScreenSizeScaled = true;
			ArrowComponent->bUseInEditorScaling = true;
		}

		if (GetSpriteComponent())
		{
			GetSpriteComponent()->Sprite = ConstructorStatics.SpriteTexture.Get();
			GetSpriteComponent()->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
			GetSpriteComponent()->SpriteInfo.Category = ConstructorStatics.ID_Wind;
			GetSpriteComponent()->SpriteInfo.DisplayName = ConstructorStatics.NAME_Wind;
			GetSpriteComponent()->SetupAttachment(Component);
		}
	}
#endif // WITH_EDITORONLY_DATA
}

UVDWindDirectionalSourceComponent* AVDWindDirectionalSourceActor::GetComponent() const 
{ 
	return Component; 
}
#if WITH_EDITORONLY_DATA
UArrowComponent* AVDWindDirectionalSourceActor::GetArrowComponent() const 
{ 
	return ArrowComponent; 
}
#endif