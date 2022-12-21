// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class WebService : ModuleRules
{
	public WebService(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "BUITween" });
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"WebService/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "WebService/Private",
		});
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" ,	"Slate", "SlateCore",
			"OnlineSubsystem", "OnlineSubsystemUtils",
			"PlayFabCommon", "PlayFabCpp" , "PlayFab",
			"Http","Json", "JsonUtilities",
		});
		
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemIOS", "IOSAdvertising" });
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
