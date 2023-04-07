// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace FGlobalVariable
{
	static const int DEAD_PREV_STAGE = 8;
	
	static const int MOB_PREUSECOUNT = 2;
	
	static const int MOB_USECOUNT = 2;
	
	static const int STAGE_MAX = 20;
	
	static const int LEVEL_MAX = 100;

	static const int SKILL_LEVEL_MAX = 10;

	static const float HERO_DEFAULT_SPEED = 400;

	static const float MONSTER_DEFAULT = 230;
}


DECLARE_MULTICAST_DELEGATE(FVoidvoidMulti);

DECLARE_DELEGATE(FVoidvoid);