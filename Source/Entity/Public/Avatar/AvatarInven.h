// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"

/**
 * 
 */
class ENTITY_API AvatarInven
{
public:
	AvatarInven();
	~AvatarInven();

protected:
	FPrimaryAssetId m_CurrentID;

protected:
	void DeselectAvatar(FStreamableDelegate deSelect);

	void SelectAvatar(FPrimaryAssetId id, FStreamableDelegate dele);
	
public:
	void ChangeAvatar(FPrimaryAssetId selectId, FStreamableDelegate onSelect);

};
