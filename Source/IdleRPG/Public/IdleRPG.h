// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace FGlobalVariable
{
	static const int LEVEL_MAX = 100;

	static const float HERO_DEFAULT_SPEED = 400.f;

	static const float HUNGER_DELAY = 4.f;
	
	static const float HUNGER_DAMAGE = 0.4f;

	static const int INVEN_SIZE = 10;//42//14

	static const int STORAGE_SIZE = 20;

	static const int ENCHANT_MAX = 20;

	static const int KARMA_MAX = 100;
	
	static const int KARMA_MIN = -100;

	static const int KARMA_FRIEND = 50;
	
	static const int KARMA_FOE = -50;

	static const int MOB_INVEN = 10;

	static const int GRID_COUNT = 15;

	static const float GRID_SIZE = 333.3f;//??188
}

class DeleterNot
{
public:
	void operator()(void*) {}
};

DECLARE_MULTICAST_DELEGATE(FVoidvoidMulti);

DECLARE_DELEGATE(FVoidvoid);