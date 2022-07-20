// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class IdleRPGEditor : ModuleRules
{
	public IdleRPGEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"IdleRPGEditor/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "IdleRPGEditor/Private",
		});
			
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Json",
				"Engine",
				"Slate",
				"SlateCore",
				"PropertyEditor",
				"UnrealEd",
				"DataTableEditor",
				"AssetTools",
				"InputCore",
				"EditorStyle",
                "IdleRPG"
            }
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Json",
				"Engine",
				"Slate",
				"SlateCore",
				"PropertyEditor",
				"UnrealEd",
				"DataTableEditor",
				"AssetTools",
				"InputCore",
				"EditorStyle",
				"IdleRPG"
            }
		);
	}
}
