// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatPawn.h"
#include "PlayerFSM.h"
#include "Actors/Components/CameraDissolve.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Avatar/AvatarAddtionalVisuals.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPlayerPawn.generated.h"

class UWidgetPawnInfoComp;
class APetPawn;
class PlayerSensor;
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
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UWidgetPawnInfoComp* m_PawnInfo;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* m_SwingSoundComp;
	UPROPERTY(VisibleAnywhere)
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_TopCamera;
	UPROPERTY()
	TArray<AActor*> m_AryIgnores;
	UPROPERTY(EditAnywhere)
	UAvatarAddtionalVisuals* m_AddVisual;

	TWeakObjectPtr<APetPawn> m_Pet;
	
	TSharedPtr<PlayerSensor> m_Sensor;

	TSharedPtr<PlayerFSM> m_Fsm;
	
	FVector m_Input;
	
	FVector m_DeltaX;
	
	FVector m_DeltaY;
	
	FVoidVoid m_OnCancelInteract;

	bool m_bCanMoveInSkill;

	bool m_bIsSkillUsing;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaSeconds) override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	virtual void StopAnimMontage() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float PlayBaseAttackAnim(float rate) override;

	void OnHpChanged();

	void OnTookDamage(ACombatPawn* other, BigInt dmg, EDamagePopup pop);

	virtual void OnDead() override;

	virtual void OnDeathAnimEnd() override;

public:
	virtual void Revive() override;
	
	virtual void SetGas(TSharedPtr<GAS> newGas) override;
	
	virtual void SetEntity(const UUnitAsset* asset) override;
	
	void TryAttack_External();
	
	bool IsInputMoving();

	void SetSkillUsing(bool b);

	bool GetSkillUsing();

	void SetCanMoveInSkill(bool b);

	bool CanMoveInSkill();

	void SetCameraCenter();

	void SetCameraTop();
	
	void SetPet(const UUnitAsset* asset);

	void UnEquipPet();

	bool IsInCombat();
};
