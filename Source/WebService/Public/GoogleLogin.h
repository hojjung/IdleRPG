// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "Entity/Public/Entity.h"
#include "OnlineSubsystemUtils.h"


/**
 * 
 */
class WEBSERVICE_API GoogleLogin
{
public:
	DECLARE_DELEGATE_OneParam(FOnLoginEnd, bool);
	
public:
	GoogleLogin(FOnLoginEnd dele);
	
	~GoogleLogin();

protected:
	FOnLoginEnd m_OnLoginEnd;
	
protected:
	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error);
	
};
