// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class WebService : ModuleRules
{
	public WebService(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"WebService/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "WebService/Private",
		});
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" ,"OnlineSubsystem", "OnlineSubsystemUtils",});
		
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemIOS", "IOSAdvertising" });
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicDependencyModuleNames.Add("OnlineSubsystemGooglePlay");
			PublicDependencyModuleNames.Add("AndroidAdvertising");
		}
	}
}
