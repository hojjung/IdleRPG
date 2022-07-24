// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class VICODynamicsPlugin_Editor : ModuleRules
	{
        public VICODynamicsPlugin_Editor(ReadOnlyTargetRules Target) : base(Target)
        {
            //PrivateIncludePaths.Add("Editor/DetailCustomizations/Private");	// For PCH includes (because they don't work with relative paths, yet)

            PrivateIncludePaths.Add("VICODynamicsPlugin_Editor/Private");

            PrivateDependencyModuleNames.AddRange(
                new string[] {
                "AppFramework",
                "Core",
                "CoreUObject",
                "DesktopWidgets",
                "Engine",
                "Landscape",
                "InputCore",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "UnrealEd",
                "EditorWidgets",
                "KismetWidgets",
                "SharedSettingsWidgets",
                "ContentBrowser",
                "BlueprintGraph",
                "AnimGraph",
                "PropertyEditor",
                "LevelEditor",
                "DesktopPlatform",
                "ClassViewer",
                "TargetPlatform",
                "ExternalImagePicker",
                "GameProjectGeneration",
                "MoviePlayer",
                "SourceControl",
                "InternationalizationSettings",
                "SourceCodeAccess",
                "RHI",
                "MediaAssets",
                "HardwareTargeting",
                "SharedSettingsWidgets",
                "VICODynamicsPlugin",
                "ComponentVisualizers"
                }
            );

            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                "Engine",
                "Media",
                "Landscape",
                "LandscapeEditor",
                "PropertyEditor",
                }
            );

            DynamicallyLoadedModuleNames.AddRange(
                new string[]
                {
                    "Layers"
                });

            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        }
	}
}