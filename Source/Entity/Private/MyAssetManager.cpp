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

UUnitAsset* UMyAssetManager::LoadUnitAsset(TSoftObjectPtr<UUnitAsset> path)
{
	TSharedPtr<FStreamableHandle> Handle;
	
	UUnitAsset* LoadedAsset = GetStreamableManager().LoadSynchronous<UUnitAsset>(path.ToSoftObjectPath(),true, &Handle);
	
	m_SetUnits.Add(Handle);

	return LoadedAsset; 
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAsset(FName id, FStreamableDelegate dele, TArray<FName> ary)
{
	TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAsset(FPrimaryAssetId(TEXT("Unit"), id), ary, dele, FStreamableManager::AsyncLoadHighPriority);

	m_SetUnits.Add(Handle);

	return Handle;
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAssetAll(FName id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;
	AryBundle.Add(TEXT("Preview"));
	AryBundle.Add(TEXT("Icon"));
	return LoadUnitAsset(id, dele, AryBundle);
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAssetIconOnly(FName id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;

	AryBundle.Add(TEXT("Icon"));

	return LoadUnitAsset(id, dele, AryBundle);
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAssetIconPreviewOnly(FName id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;
	AryBundle.Add(TEXT("Preview"));
	AryBundle.Add(TEXT("Icon"));

	return LoadUnitAsset(id, dele, AryBundle);
}

void UMyAssetManager::ClearUnits()
{
	for (auto Handle : m_SetUnits)
	{
		Handle->ReleaseHandle();
	}

	m_SetUnits.Reset();

	UKismetSystemLibrary::CollectGarbage();
}
