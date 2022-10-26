// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IdleRPG.h"

/**
 * 
 */
class IDLERPG_API LevelManager
{
public:
	LevelManager(UObject* outer);
	
	~LevelManager();

	FVoidvoid m_OnLevelLoadComplete;

protected:
	TWeakObjectPtr<UObject> m_Outer;

	FName m_CurrentLevel;

public:
	bool OpenLevel(const FName& levelName);
	
	void OnLoadComplete(const FString& mapName);
};
