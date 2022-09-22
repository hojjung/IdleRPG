// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar/AvatarInven.h"

#include "MyAssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AvatarInven::AvatarInven()
{
}

AvatarInven::~AvatarInven()
{
}

void AvatarInven::DeselectAvatar(FStreamableDelegate deSelect)
{
	TArray<FPrimaryAssetId> AryIds;

	AryIds.Add(m_CurrentID);

	TArray<FName> AryBundlesAdd;
	
	TArray<FName> AryBundlesRemove;
	AryBundlesRemove.Add(TEXT("Preview"));
	
	UMyAssetManager::Get()->ChangeBundleStateForPrimaryAssets(AryIds, AryBundlesAdd, AryBundlesRemove, false,deSelect);
}

void AvatarInven::SelectAvatar(FPrimaryAssetId id, FStreamableDelegate dele)
{
	m_CurrentID = id;
	
	TArray<FPrimaryAssetId> AryIds;

	AryIds.Add(m_CurrentID);

	TArray<FName> AryBundlesAdd;
	AryBundlesAdd.Add(TEXT("Preview"));
	
	TArray<FName> AryBundlesRemove;
	
	UMyAssetManager::Get()->ChangeBundleStateForPrimaryAssets(AryIds, AryBundlesAdd, AryBundlesRemove,false,dele);
}

void AvatarInven::OpenPreviewLevel(const UWorld* world)
{
	//UGameplayStatics::LoadStreamLevel(world, TEXT("PreviewActorScene"), true, true, LatentInfo);
}

void AvatarInven::ChangeAvatar(FPrimaryAssetId selectId, FStreamableDelegate onSelect)
{
	DeselectAvatar(FStreamableDelegate::CreateLambda(
		[=]() ->void
		{
			SelectAvatar(selectId, onSelect);
			UKismetSystemLibrary::CollectGarbage();
		}
		));
}

void AvatarInven::SpawnPreviewActor(UWorld* w)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	
	m_PreviewActor = w->SpawnActor<APreviewActor>(APreviewActor::StaticClass(),FVector(9999,9999,9999),FRotator(0),Param);

	m_PreviewActor->HideMeshWithTick();
}

void AvatarInven::SetPreview(const UUnitAsset* asset)
{
	m_PreviewActor->SetEntity(asset);

	m_PreviewActor->ShowMeshWithTick();
}

void AvatarInven::HidePreview()
{
	m_PreviewActor->HideMeshWithTick();
}

APreviewActor* AvatarInven::GetPreviewActor()
{
	return m_PreviewActor.Get();
}
