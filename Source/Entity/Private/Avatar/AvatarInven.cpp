// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar/AvatarInven.h"

#include "MyAssetManager.h"

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

void AvatarInven::ChangeAvatar(FPrimaryAssetId selectId, FStreamableDelegate onSelect)
{
	DeselectAvatar(FStreamableDelegate::CreateLambda(
		[=]() ->void
		{
			SelectAvatar(selectId, onSelect);
		}
		));
}
