// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class Entity : ModuleRules
{
	public Entity(ReadOnlyTargetRules Target) : base(Target)
	{
		
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"Entity/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "Entity/Private",
		});
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core","CoreUObject", "Engine", "NavigationSystem","AIModule" , "UMG"});
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate","SlateCore" });
		
	}
}
