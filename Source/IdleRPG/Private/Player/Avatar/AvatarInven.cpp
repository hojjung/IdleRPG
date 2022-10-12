#include "Player/Avatar/AvatarInven.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

AvatarInven::AvatarInven()
{
	m_EquippedAvatar = TEXT("Avatar_39");
}

AvatarInven::~AvatarInven()
{
}

void AvatarInven::DeselectAvatar(FStreamableDelegate deSelect)
{
	TArray<FPrimaryAssetId> AryIds;

	AryIds.Add(m_CurrentPreviewID);

	TArray<FName> AryBundlesAdd;
	
	TArray<FName> AryBundlesRemove;
	AryBundlesRemove.Add(TEXT("Preview"));
	
	UMyAssetManager::Get()->ChangeBundleStateForPrimaryAssets(AryIds, AryBundlesAdd, AryBundlesRemove, false,deSelect);
}

void AvatarInven::SelectAvatar(FPrimaryAssetId id, FStreamableDelegate dele)
{
	m_CurrentPreviewID = id;
	
	TArray<FPrimaryAssetId> AryIds;

	AryIds.Add(m_CurrentPreviewID);

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

void AvatarInven::SetPreview(FName key, const UUnitAsset* asset)
{
	m_PreviewID = key;
	
	m_PreviewActor->SetEntity(asset);

	ShowPreview();
}

void AvatarInven::ShowPreview()
{
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

void AvatarInven::EquipAvatar()
{
	if(m_PreviewID == NAME_None)
	{
		return;
	}
	m_EquippedAvatar = m_PreviewID;

	if(m_EquipSkinAvatar == NAME_None)
	{
		m_EquipSkinAvatar = m_EquippedAvatar;
	}

	UpdateEquipAvatar();
}

void AvatarInven::EquipSkinAvatar()
{
	if(m_PreviewID == NAME_None)
	{
		return;
	}
	m_EquipSkinAvatar = m_PreviewID;

	UpdateEquipAvatar();
}

void AvatarInven::UpdateEquipAvatar()
{
	if(!UMyGameInstance::Get->m_Player.Get())
	{
		return;
	}

	FName Skin = m_EquippedAvatar;
	
	if(m_EquipSkinAvatar != NAME_None)
	{
		Skin = m_EquipSkinAvatar;
	}

	const FAvatarRow& Avatar = *UAvatarData::GetAvatarTable->FindRow<FAvatarRow>(Skin, "");

	UAssetManager* Manager = UAssetManager::GetIfValid();
	
	UUnitAsset* Asset = Cast<UUnitAsset>(Manager->GetPrimaryAssetObject(Avatar.m_EntityAsset));

	UMyGameInstance::Get->m_Player->SetEntity(Asset);
}

void AvatarInven::InitEquipAvatar()
{
	if(!UMyGameInstance::Get->m_Player.Get())
	{
		return;
	}

	FName Skin = m_EquippedAvatar;
	
	if(m_EquipSkinAvatar != NAME_None)
	{
		Skin = m_EquipSkinAvatar;
	}

	const FAvatarRow& Avatar = *UAvatarData::GetAvatarTable->FindRow<FAvatarRow>(Skin, "");

	SelectAvatar(Avatar.m_EntityAsset, FStreamableDelegate::CreateLambda(
		[=]
		{
			UAssetManager* Manager = UAssetManager::GetIfValid();
	
			UUnitAsset* Asset = Cast<UUnitAsset>(Manager->GetPrimaryAssetObject(Avatar.m_EntityAsset));
			
			UMyGameInstance::Get->m_Player->SetEntity(Asset);		
		}
		));
}
