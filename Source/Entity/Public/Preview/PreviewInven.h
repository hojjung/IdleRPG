// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PreviewActor.h"
#include "Engine/StreamableManager.h"

/**
 * 
 */
class ENTITY_API PreviewInven
{
public:
	PreviewInven();
	
	virtual ~PreviewInven();

protected:
	TWeakObjectPtr<APreviewActor> m_PreviewActor;

	FPrimaryAssetId m_CurrentPreviewID;

	FName m_PreviewID;
	
protected:
	void DeselectAvatar(FStreamableDelegate deSelect);

	void SelectAvatar(FPrimaryAssetId id, FStreamableDelegate dele);

public:
	void ChangeAvatar(FPrimaryAssetId selectId, FStreamableDelegate onSelect);

	void SpawnPreviewActor(UWorld* w);

	void SetPreview(FName key, const UUnitAsset* asset);

	void ShowPreview();
	
	void HidePreview();

	APreviewActor* GetPreviewActor();
};
