// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class ItemInventory : ModuleRules
{
	public ItemInventory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"ItemInventory/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "ItemInventory/Private",
		});
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "Engine", "Entity" });
	}
}
