// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class Crafting : ModuleRules
{
	public Crafting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"Crafting/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "Crafting/Private",
		});
			
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "ItemInventory" });
	}
}
