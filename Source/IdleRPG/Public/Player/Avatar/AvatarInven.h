// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PreviewActor.h"
#include "Engine/StreamableManager.h"

/**
 * 
 */
class IDLERPG_API AvatarInven
{
public:
	AvatarInven();
	
	~AvatarInven();

protected:
	TWeakObjectPtr<APreviewActor> m_PreviewActor;
	
	FPrimaryAssetId m_CurrentID;

protected:
	void DeselectAvatar(FStreamableDelegate deSelect);

	void SelectAvatar(FPrimaryAssetId id, FStreamableDelegate dele);

	void OpenPreviewLevel(const UWorld* world);
	
public:
	void ChangeAvatar(FPrimaryAssetId selectId, FStreamableDelegate onSelect);

	void SpawnPreviewActor(UWorld* w);

	void SetPreview(const UUnitAsset* asset);

	void ShowPreview();
	
	void HidePreview();

	APreviewActor* GetPreviewActor();
};
