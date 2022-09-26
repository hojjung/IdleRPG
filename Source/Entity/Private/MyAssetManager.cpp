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
	TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAsset(id, ary, dele, FStreamableManager::AsyncLoadHighPriority);
    // this block is required an subscription to load complete or stalled. Now this blocking thread.
    {
        // Get initial loading state.
        EAsyncPackageState::Type LoadState = EAsyncPackageState::TimeOut;
 
        // Get waiting for timeout if is 
        while(LoadState != EAsyncPackageState::Complete && Handle->IsActive())
        {
            // Get Asset Object ready for async loading.
            LoadState = Handle->WaitUntilComplete(0.f /*Forever*/, true/*Force load*/);
 
            if(LoadState == EAsyncPackageState::PendingImports)
            {
                // exit from loading asset with invalid resolving object. wiil be promote UNRESOLVED error.
                LoadState = EAsyncPackageState::Complete;
            	PRINTF("AssetManager Pending");
            }
        }
    }
	UUnitAsset* Unit = Cast<UUnitAsset>(Handle->GetLoadedAsset());
	
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
