// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotify_Trigger.h"

#include "Player/CombatPawn.h"

void UAnimNotify_Trigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(!MeshComp->GetOwner())
	{
		return;
	}

	ACombatPawn* Pawn =  Cast<ACombatPawn>( MeshComp->GetOwner());

	if(!Pawn)
	{
		return;
	}

	Pawn->OnNotifyTrigger(m_ID);
}
