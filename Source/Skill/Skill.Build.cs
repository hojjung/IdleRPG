// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class Skill : ModuleRules
{
	public Skill(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"Skill/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "Skill/Private",
		});
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Entity" });
	}
}
