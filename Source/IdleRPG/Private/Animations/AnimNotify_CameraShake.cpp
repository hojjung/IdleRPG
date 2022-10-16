// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotify_CameraShake.h"

#include "MyGameInstance.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(!m_ClassCamShake)
	{
		return;
	}

	UWorld* const World = GEngine->GetWorldFromContextObjectChecked(MeshComp);

	if(World->WorldType == EWorldType::EditorPreview)
	{
		return;
	}
     
	UMyGameInstance::Get->GetPlayerCon()->ClientStartCameraShake(m_ClassCamShake);	
}
