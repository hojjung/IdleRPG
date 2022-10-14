// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyCheatManager.h"

#include "MyGameInstance.h"

void UMyCheatManager::SpawnDefaultMap(int level)
{
	UMyGameInstance::Get->StartGameMode(EGameMode::Default, level);	
}
