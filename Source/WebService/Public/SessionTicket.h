// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayFabAuthenticationContext.h"
#include "SessionTicket.generated.h"

/**
 * 
 */
UCLASS()
class WEBSERVICE_API USessionTicket : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString m_SessionTicket;

public:
	static TSharedPtr<UPlayFabAuthenticationContext> CreateAuthCon(const FString* newSessonTicket = nullptr);
};
