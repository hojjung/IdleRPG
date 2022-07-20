// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class IdleRPGTarget : TargetRules
{
	public IdleRPGTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "IdleRPG" } );
		ExtraModuleNames.AddRange( new string[] { "Entity" } );
		ExtraModuleNames.AddRange( new string[] { "ItemInventory" } );
		ExtraModuleNames.AddRange( new string[] { "Crafting" } );
		ExtraModuleNames.AddRange( new string[] { "Enchant" } );
		ExtraModuleNames.AddRange( new string[] { "Skill" } );
		ExtraModuleNames.AddRange( new string[] { "WebService" } );
		ExtraModuleNames.AddRange( new string[] { "LevelManage" } );
	}
}
