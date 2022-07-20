// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class Enchant : ModuleRules
{
	public Enchant(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"Enchant/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "Enchant/Private",
		});
			
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		PrivateDependencyModuleNames.AddRange(new string[] { "ItemInventory" });
	}
}
