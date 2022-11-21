// Fill out your copyright notice in the Description page of Project Settings.
// FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, MonsterId, SpawnLocation);


#include "MyAssetManager.h"

#include "Entity.h"
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

UUnitAsset* UMyAssetManager::LoadUnitAsset(FPrimaryAssetId id, FStreamableDelegate dele, TArray<FName> ary)
{
	TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAsset(id, ary, FStreamableDelegate(),
	                                                        FStreamableManager::AsyncLoadHighPriority);

	UUnitAsset* Unit = nullptr;

	UMyAssetManager::SyncLoad(dele, Handle);

	if (Handle != nullptr)
	{
		Unit = Cast<UUnitAsset>(Handle->GetLoadedAsset());
	}
	else
	{
		Unit = Cast<UUnitAsset>(GetPrimaryAssetObject(id));
	}
	return Unit;
}

UUnitAsset* UMyAssetManager::LoadUnitAssetMeshOnly(FPrimaryAssetId id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;
	AryBundle.Add(TEXT("Preview"));
	AryBundle.Add(TEXT("Default"));

	return LoadUnitAsset(id, dele, AryBundle);
}

UUnitAsset* UMyAssetManager::LoadUnitAssetIconOnly(FPrimaryAssetId id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;

	AryBundle.Add(TEXT("Icon"));

	return LoadUnitAsset(id, dele, AryBundle);
}

UUnitAsset* UMyAssetManager::LoadUnitAssetIconPreviewOnly(FPrimaryAssetId id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;
	AryBundle.Add(TEXT("Preview"));
	AryBundle.Add(TEXT("Icon"));

	return LoadUnitAsset(id, dele, AryBundle);
}

void UMyAssetManager::ClearUnits()
{
	UKismetSystemLibrary::CollectGarbage();
}

void UMyAssetManager::SyncLoad(FStreamableDelegate dele, TSharedPtr<FStreamableHandle> Handle)
{
	if (!Handle.Get())
	{
		dele.ExecuteIfBound();
		return;
	}
	EAsyncPackageState::Type LoadState = EAsyncPackageState::TimeOut;
	// Get waiting for timeout if is 
	while (LoadState != EAsyncPackageState::Complete && Handle->IsActive())
	{
		// Get Asset Object ready for async loading.
		LoadState = Handle->WaitUntilComplete(0.f /*Forever*/, true/*Force load*/);

		if (LoadState == EAsyncPackageState::PendingImports)
		{
			LoadState = EAsyncPackageState::Complete;
		}
	}
	dele.ExecuteIfBound();
}
