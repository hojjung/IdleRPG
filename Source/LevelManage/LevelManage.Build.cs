// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class LevelManage : ModuleRules
{
	public LevelManage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"LevelManage/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "LevelManage/Private",
		});
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
	}
}
