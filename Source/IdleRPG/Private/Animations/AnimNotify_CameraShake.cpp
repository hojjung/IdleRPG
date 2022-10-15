// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotify_CameraShake.h"

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
     
	// if(World->WorldType != EWorldType::Game)
	// {
	// 	return;
	// }
	
	UMyGameinstance::Get->GetPlayerCon()->ClientStartCameraShake(m_ClassCamShake);	
}
