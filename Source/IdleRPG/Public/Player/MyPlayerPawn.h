// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Engine/StreamableManager.h"
#include "Monsters/QuadTree.h"
#include "MyPlayerPawn.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API AMyPlayerPawn : public AMyBasePawn
{
	GENERATED_BODY()

protected:
	TSharedPtr<FStreamableHandle> m_Asset;
	
	TSharedPtr<QuadTree> m_QuadTree;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void OnLoaded();

	virtual void Tick(float DeltaSeconds) override;
};
