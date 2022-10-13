// Fill out your copyright notice in the Description page of Project Settings.


#include "Pet/PetPawn.h"

#include "IdleRPG.h"
#include "MyLib.h"
#include "Actors/Components/MyNavMovement.h"


APetPawn::APetPawn(const FObjectInitializer& objInit):Super(objInit)
{
	m_BodyMesh->SetRelativeLocation(FVector(0,0,100));
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void APetPawn::SetPetEntity(const UUnitAsset* pet_row)
{
	m_PFComp->SetMovementComponent(m_Movement);	
	m_PFComp->Initialize();
	m_Movement->SetPathFollowingAgent(m_PFComp);
	m_Movement->MaxSpeed = FGlobalVariable::HERO_DEFAULT_SPEED * 0.99f;

	LoadSetSkMeshAnim(pet_row);

	m_BodyMesh->SetRelativeScale3D(FVector(pet_row->m_fScale));

	m_PlayerPawn = UMyLib::GetPlayer();

	m_fInitZ = GetSkMesh()->GetRelativeLocation().Z;

	m_fFloatingZ = 10;

	m_fMaxZ = m_fInitZ + m_fFloatingZ;

	m_fMinZ = m_fInitZ - m_fFloatingZ;

	m_bIsMax = true;

	m_fTimer = 0;

	m_fFloatingTime = 0.6f;

	SetActorTickEnabled(true);
}

void APetPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AnimateFly(DeltaSeconds);

	MoveToActor(m_PlayerPawn.Get(),100);

	HomingRotateToTarget(5, m_PlayerPawn.Get()->GetActorLocation());
}

void APetPawn::AnimateFly(float DeltaSeconds)
{
	float LerpZ = FMath::Lerp<float>(m_bIsMax ? m_fMinZ : m_fMaxZ,m_bIsMax ? m_fMaxZ : m_fMinZ,m_fTimer / m_fFloatingTime);

	m_BodyMesh->SetRelativeLocation(FVector(0,0,LerpZ));

	m_fTimer += DeltaSeconds;

	if(m_fTimer >= m_fFloatingTime)
	{
		m_fTimer = 0;

		m_bIsMax = !m_bIsMax;
	}
}