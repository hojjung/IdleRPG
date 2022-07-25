// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/EntityBaseAsset.h"
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

protected:
	TSet<TSharedPtr<FStreamableHandle>> m_SetUnits;

public:
	TSharedPtr<FStreamableHandle> LoadUnitAsset(FName id, FStreamableDelegate dele, TArray<FName> ary);
	
	TSharedPtr<FStreamableHandle> LoadUnitAssetAll(FName id, FStreamableDelegate dele);

	TSharedPtr<FStreamableHandle> LoadUnitAssetIconOnly(FName id, FStreamableDelegate dele);

	TSharedPtr<FStreamableHandle> LoadUnitAssetIconPreviewOnly(FName id, FStreamableDelegate dele);
	
	void ClearUnits();
};


