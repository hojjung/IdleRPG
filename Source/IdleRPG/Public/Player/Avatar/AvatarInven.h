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

	FPrimaryAssetId m_CurrentPreviewID;
	
	FName m_PreviewID;
	
	FName m_EquippedAvatar;

	FName m_EquipSkinAvatar;
	

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

	void EquipAvatar();

	void EquipSkinAvatar();

	void UpdateEquipAvatar();

	void InitEquipAvatar();
};
