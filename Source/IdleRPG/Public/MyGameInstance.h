// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	static UMyGameInstance* Get;

public:
	//TWeakObjectPtr<AMyPlayerPawn> m_Player;
	
	//TWeakObjectPtr<AMyPlayerController> m_PlayerCon;
	
protected:
	virtual void BeginDestroy() override;

public:
	virtual void Init() override;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	
	void Tick(float deltaTime);
};


