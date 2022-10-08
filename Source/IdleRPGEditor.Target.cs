// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class IdleRPGEditorTarget : TargetRules
{
	public IdleRPGEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "IdleRPG" } );
		ExtraModuleNames.AddRange( new string[] { "IdleRPGEditor" } );
		ExtraModuleNames.AddRange( new string[] { "Entity" } );
		ExtraModuleNames.AddRange( new string[] { "WebService" } );
	}
}
