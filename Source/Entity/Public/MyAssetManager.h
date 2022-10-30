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
	UUnitAsset* LoadUnitAsset(FPrimaryAssetId id, FStreamableDelegate dele, TArray<FName> ary);
	
public:
	UUnitAsset* LoadUnitAssetMeshOnly(FPrimaryAssetId id, FStreamableDelegate dele);
	
	UUnitAsset* LoadUnitAssetIconOnly(FPrimaryAssetId id, FStreamableDelegate dele);

	UUnitAsset* LoadUnitAssetIconPreviewOnly(FPrimaryAssetId id, FStreamableDelegate dele);
	
	void ClearUnits();

	static void SyncLoad(FStreamableDelegate dele, TSharedPtr<FStreamableHandle> Handle);
};


