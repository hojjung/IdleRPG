// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WebService/Public/GoogleLogin.h"
#include "GameFramework/GameModeBase.h"
#include "IdleRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API AIdleRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIdleRPGGameModeBase();

protected:
	TSharedPtr<GoogleLogin> m_GoogleLogin;

protected:
	virtual void BeginPlay() override;

	virtual void StartPlay() override;

	void LoginEnd(bool bSuccess);

	virtual void Tick(float DeltaSeconds) override;
};
