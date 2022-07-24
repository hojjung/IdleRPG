// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"

/**
 * 
 */
class IDLERPG_API SpawnManager
{
public:
	SpawnManager();
	~SpawnManager();

protected:
	TArray<TSharedPtr<Monster>> m_AryMonsters;

public:
	void Update(float delta);
};
