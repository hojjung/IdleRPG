
// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class IdleRPG : ModuleRules
{
	public IdleRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Entity", "NavigationSystem","AIModule","BUITween","GameplayCameras",
			"PlayFabCommon", "PlayFabCpp" , "PlayFab"});
		
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
			"Slate", "SlateCore"
		});

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemIOS", "IOSAdvertising" });
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.Add("AndroidPermission");
			PrivateDependencyModuleNames.Add("OnlineSubsystemGooglePlay");
			PrivateDependencyModuleNames.Add("AndroidAdvertising");
			
			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Android_UPL.xml"));
		}
	}
}
