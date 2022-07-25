// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatPawn.h"
#include "Actors/Components/CameraDissolve.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Camera/CameraComponent.h"
#include "Engine/StreamableManager.h"
#include "Monsters/QuadTree.h"
#include "MyPlayerPawn.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API AMyPlayerPawn : public ACombatPawn
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FVoidVoid);

	
public:
	AMyPlayerPawn(const FObjectInitializer& objInit);
	
protected:
	UPROPERTY(VisibleAnywhere)
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_TopCamera;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;
	UPROPERTY()
	TArray<AActor*> m_AryIgnores;
	
	TSharedPtr<FStreamableHandle> m_Asset;
	
	FVector m_Input;
	
	FVector m_DeltaX;
	
	FVector m_DeltaY;
	
	FAIRequestID m_ReqID;

	FVoidVoid m_OnRequestDone;

	FVoidVoid m_OnCancelInteract;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void OnLoaded();

	virtual void Tick(float DeltaSeconds) override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void OnRequestMoveDone(FAIRequestID id, const FPathFollowingResult& rslt);

	void TryAttack_External();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsInputMoving();

	void RequestAttack();

	void RequestInteract(AActor* target, const FVoidVoid& delegate, float r);
};
