// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "Navigation/PathFollowingComponent.h"
#include "MyBasePawn.generated.h"

class UMyNavMovement;
struct FNpcUnitEntityRow;
class UUnitEntityAsset;
UCLASS()
class ENTITY_API AMyBasePawn : public APawn
{
	GENERATED_BODY()


public:
	AMyBasePawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Character)
	UCapsuleComponent* m_Capsule;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Character)
	USkeletalMeshComponent* m_BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "AI")
	UPathFollowingComponent* m_PFComp;//UCrowdFollowingComponent
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Movement")
	UMyNavMovement* m_Movement;
	
	TSoftObjectPtr<UUnitEntityAsset> m_EntityAsset;

	FTimerHandle m_MoveStopTimer;
	
	FText m_PawnName;

	FName m_EntityID;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void LoadSetSkMeshAnim(TSoftObjectPtr<UUnitEntityAsset> asset);

	void ActiveMovement();
	//PathFollow	
	FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

	void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query,FNavPathSharedPtr& OutPath) const;

	bool BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const;

	FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path);

	USkeletalMeshComponent* CreateSkMeshComp(FName keyID);
public:
	virtual void SetEntity(const FName& id, const FNpcUnitEntityRow& unitEntityRow);//Init
	
	virtual FPathFollowingRequestResult MoveToLocation(FVector loc, float acceptRadius = 0.f);

	virtual FPathFollowingRequestResult MoveToActor(AActor* target, float acceptRadius = 0);

	bool LineOfSightTo(const AActor* Other) const;

public://Anim
	float PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	void StopAnimMontage();

	UAnimMontage* GetCurrentMontage();

public:
	UCapsuleComponent* GetCapsule() const;

	USkeletalMeshComponent* GetSkMesh() const;

	UPathFollowingComponent* GetPfComp() const;

	UMyNavMovement* GetMove() const;

	FText GetPawnName() const;

	virtual bool IsMoving() const;

	void ClearStopMoveDelegate();
public:
	float GetRadius() const;
	
	void StopMove();
	
	virtual bool IsRange();

	const FName& GetEntityID() const;

	void SetActorFeetLocation(FVector loc);

	virtual FVector GetNavAgentLocation() const override;
};
