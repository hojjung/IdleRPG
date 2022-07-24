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

protected:
	TSharedPtr<GoogleLogin> m_GoogleLogin;
	
protected:
	virtual void BeginPlay() override;

	void LoginEnd(bool bSuccess);
};
