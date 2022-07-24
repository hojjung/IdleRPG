// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetManager.h"

#include "Kismet/KismetSystemLibrary.h"

UMyAssetManager* UMyAssetManager::Get()
{
	UMyAssetManager* This = Cast<UMyAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return This;
	}
	else
	{
		return nullptr; 
	}
}

const UUnitEntityAsset* UMyAssetManager::LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> asset)
{
	TSharedPtr<FStreamableHandle> Handle;
	
	UUnitEntityAsset* LoadedAsset = GetStreamableManager().LoadSynchronous<UUnitEntityAsset>(asset.ToSoftObjectPath(),true, &Handle);
	
	m_SetUnits.Add(Handle);

	return LoadedAsset; 
}

void UMyAssetManager::ClearUnits()
{
	for(auto Handle : m_SetUnits)
	{
		Handle->ReleaseHandle();
	}

	m_SetUnits.Reset();

	UKismetSystemLibrary::CollectGarbage();
}

