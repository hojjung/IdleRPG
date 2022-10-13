
#include "Preview/PreviewInven.h"

#include "Engine/AssetManager.h"
#include "Kismet/KismetSystemLibrary.h"


PreviewInven::PreviewInven()
{
	
}

PreviewInven::~PreviewInven()
{
	
}

void PreviewInven::DeselectAvatar(FStreamableDelegate deSelect)
{
	TArray<FPrimaryAssetId> AryIds;

	AryIds.Add(m_CurrentPreviewID);

	TArray<FName> AryBundlesAdd;
	
	TArray<FName> AryBundlesRemove;
	AryBundlesRemove.Add(TEXT("Preview"));

	UAssetManager::Get().ChangeBundleStateForPrimaryAssets(AryIds, AryBundlesAdd, AryBundlesRemove, false,deSelect);
}

void PreviewInven::SelectAvatar(FPrimaryAssetId id, FStreamableDelegate dele)
{
	m_CurrentPreviewID = id;
	
	TArray<FPrimaryAssetId> AryIds;

	AryIds.Add(m_CurrentPreviewID);

	TArray<FName> AryBundlesAdd;
	AryBundlesAdd.Add(TEXT("Preview"));
	
	TArray<FName> AryBundlesRemove;
	
	UAssetManager::Get().ChangeBundleStateForPrimaryAssets(AryIds, AryBundlesAdd, AryBundlesRemove,false,dele);
}

void PreviewInven::ChangeAvatar(FPrimaryAssetId selectId, FStreamableDelegate onSelect)
{
	DeselectAvatar(FStreamableDelegate::CreateLambda(
		[=]() ->void
		{
			SelectAvatar(selectId, onSelect);
			UKismetSystemLibrary::CollectGarbage();
		}
		));
}

void PreviewInven::SpawnPreviewActor(UWorld* w, float camSize, float zOffset)
{
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	
	m_PreviewActor = w->SpawnActor<APreviewActor>(APreviewActor::StaticClass(),FVector(9999,9999,9999),FRotator(0),Param);

	m_PreviewActor->HideMeshWithTick();

	m_PreviewActor->SetCamSize(camSize);

	m_PreviewActor->SetZOffset(zOffset);
}

void PreviewInven::SetPreview(FName key, const UUnitAsset* asset)
{
	m_PreviewID = key;
	
	m_PreviewActor->SetEntity(asset);

	ShowPreview();
}

void PreviewInven::ShowPreview()
{
	m_PreviewActor->ShowMeshWithTick();
}

void PreviewInven::HidePreview()
{
	m_PreviewActor->HideMeshWithTick();
}

APreviewActor* PreviewInven::GetPreviewActor()
{
	return m_PreviewActor.Get();
}
