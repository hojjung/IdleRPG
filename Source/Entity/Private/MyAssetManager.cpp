// Fill out your copyright notice in the Description page of Project Settings.
// FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, MonsterId, SpawnLocation);
    

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

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAsset(FPrimaryAssetId id, FStreamableDelegate dele, TArray<FName> ary)
{
	TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAsset(id, ary, dele, FStreamableManager::AsyncLoadHighPriority);

	EAsyncPackageState::Type Result = Handle->WaitUntilComplete();

	if(Result == EAsyncPackageState::Complete)
	{
		dele.Execute();
		dele.Unbind();
		Handle->CancelHandle();
	}

	m_SetUnits.Add(Handle);
	//한 애셋을 동시에 로드 할때, 먼저 요청한게 덮어 씌워저버림.

	//번들 스테이드가 다르면 어떡하지?\

	//번들이 다를경우, 전부 로드가 되야지 맞다.

	//Handle.Get()->WaitUntilComplete()

	return Handle;
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAssetMeshOnly(FPrimaryAssetId id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;
	AryBundle.Add(TEXT("Preview"));
	AryBundle.Add(TEXT("Default"));

	return LoadUnitAsset(id, dele, AryBundle);
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAssetIconOnly(FPrimaryAssetId id, FStreamableDelegate dele)
{
	TArray<FName> AryBundle;

	AryBundle.Add(TEXT("Icon"));

	return LoadUnitAsset(id, dele, AryBundle);
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadUnitAssetIconPreviewOnly(FPrimaryAssetId id, FStreamableDelegate dele)
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
