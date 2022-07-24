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
	const UUnitEntityAsset* LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> asset);
	
	void ClearUnits();
};
