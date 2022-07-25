// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerPawn.h"

#include "MyAssetManager.h"
#include "Monsters/QuadTree.h"

void AMyPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	m_Asset = UMyAssetManager::Get()->LoadUnitAssetAll(TEXT("Player"),
		FStreamableDelegate::CreateUObject(this, &AMyPlayerPawn::OnLoaded));	
}

void AMyPlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_QuadTree.Reset();
}

void AMyPlayerPawn::OnLoaded()
{
	UUnitEntityAsset* Asset = Cast<UUnitEntityAsset>(m_Asset.Get()->GetLoadedAsset());

	SetEntity(Asset);
}

void AMyPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
