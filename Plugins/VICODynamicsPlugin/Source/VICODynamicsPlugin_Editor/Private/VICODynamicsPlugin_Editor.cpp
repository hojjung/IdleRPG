// Copyright 2012-2017 VICO Game Studio LLC. All Rights Reserved.

#include "VICODynamicsPlugin_Editor.h"
#include "VICODynamicsPlugin_EditorPrivatePCH.h"
#include "VDComponentCustomization.h"

#include "ComponentVisualizers.h"
#include "VDComponentVisualizer.h"
#include "VDRopeComponent.h"
#include "VDDynamicRopeComponent.h"
#include "VDSplineRopeComponent.h"
#include "VDSkinnedRopeComponent.h"
#include "VDProceduralClothComponent.h"
#include "VDMeshClothComponent.h"


class FVICODynamicsPlugin_Editor : public IVICODynamicsPlugin_Editor
{
public:
	FVICODynamicsPlugin_Editor()
	{

	}

	virtual void StartupModule() override
	{
		static const FName PropertyEditor("PropertyEditor");
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
		PropertyModule.RegisterCustomPropertyTypeLayout("VDCollisionSettings", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FVDComponentCustomization::MakeInstance));
		PropertyModule.NotifyCustomizationModuleChanged();

		static const FName CompVisualizers = TEXT("ComponentVisualizers");
		auto VisualizersModule = FModuleManager::GetModulePtr<FComponentVisualizersModule>(CompVisualizers);
		if (VisualizersModule)
		{
			VisualizersModule->RegisterComponentVisualizer(UVDRopeComponent::StaticClass()->GetFName(), MakeShareable(new FVDComponentVisualizer()));
			VisualizersModule->RegisterComponentVisualizer(UVDDynamicRopeComponent::StaticClass()->GetFName(), MakeShareable(new FVDComponentVisualizer()));
			VisualizersModule->RegisterComponentVisualizer(UVDSplineRopeComponent::StaticClass()->GetFName(), MakeShareable(new FVDComponentVisualizer()));
			VisualizersModule->RegisterComponentVisualizer(UVDSkinnedRopeComponent::StaticClass()->GetFName(), MakeShareable(new FVDComponentVisualizer()));
			VisualizersModule->RegisterComponentVisualizer(UVDProceduralClothComponent::StaticClass()->GetFName(), MakeShareable(new FVDComponentVisualizer()));
			VisualizersModule->RegisterComponentVisualizer(UVDMeshClothComponent::StaticClass()->GetFName(), MakeShareable(new FVDComponentVisualizer()));
		}
	}

	virtual void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");						
			PropertyModule.UnregisterCustomPropertyTypeLayout("VDCollisionSettings");
			PropertyModule.NotifyCustomizationModuleChanged();
		}
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};

IMPLEMENT_MODULE(FVICODynamicsPlugin_Editor, VICODynamicsPlugin_Editor);
