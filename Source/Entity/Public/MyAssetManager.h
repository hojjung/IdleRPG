// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/UnitAsset.h"
#include "Engine/AssetManager.h"
#include "MyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UMyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UMyAssetManager() {}
	
	static UMyAssetManager* Get();

private:
	TSet<TSharedPtr<FStreamableHandle>> m_SetUnits;

private:
	TSharedPtr<FStreamableHandle> LoadUnitAsset(FPrimaryAssetId id, FStreamableDelegate dele, TArray<FName> ary);
	
public:
	TSharedPtr<FStreamableHandle> LoadUnitAssetMeshOnly(FPrimaryAssetId id, FStreamableDelegate dele);
	
	TSharedPtr<FStreamableHandle> LoadUnitAssetIconOnly(FPrimaryAssetId id, FStreamableDelegate dele);

	TSharedPtr<FStreamableHandle> LoadUnitAssetIconPreviewOnly(FPrimaryAssetId id, FStreamableDelegate dele);
	
	void ClearUnits();
};


