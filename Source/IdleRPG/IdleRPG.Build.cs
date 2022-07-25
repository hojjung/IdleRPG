
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IdleRPG : ModuleRules
{
	public IdleRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"IdleRPG/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
			"IdleRPG/Private",
		});
	
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "WebService",
			"Slate", "SlateCore", "Entity","Engine"
		});

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemIOS", "IOSAdvertising" });
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.Add("OnlineSubsystemGooglePlay");
			PrivateDependencyModuleNames.Add("AndroidAdvertising");
		}
	}
}
