// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "GAS/GAS.h"
#include "CombatPawn.generated.h"

UENUM(BlueprintType)
enum class EDmgType : uint8
{
	DmgPhys
	
};

UCLASS()
class IDLERPG_API ACombatPawn : public AMyBasePawn
{
	GENERATED_BODY()

public:
	ACombatPawn(const FObjectInitializer& objInit);

public:
	DECLARE_DELEGATE_OneParam(FOnDied, const ACombatPawn* );

	FOnDied m_OnDied;
	
protected:
	TWeakPtr<GAS> m_Gas;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_ShadowMeshComp;
	
	TWeakObjectPtr<ACombatPawn> m_Target;
	
	bool m_bIsRotateable;

	float m_fAttackCD;

	float m_fAtkRange;

	float m_fAtkRangeSqr;
	
protected:
	float PlayBaseAttackAnim();

	void SetAtkRange(float v);

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnTickAlive(float DeltaSeconds);

public:
	void SetGas(TSharedPtr<GAS> newGas);
	
	virtual void SetEntity(const UUnitAsset* asset) override;
	
	void SetFocusedTarget(ACombatPawn* pawn);
	
	ACombatPawn* GetFocusedTarget();
	
	float TryAttack(float playRate = 1.f);

	
	virtual bool IsRangeMode() {return true;};

	float GetAttackRange();

	float GetAttackRangeSqr();

	EPathFollowingRequestResult::Type ChaseTarget();

	virtual  float MyTakeDamage(float DamageAmount, EDmgType dmgType, ACombatPawn* DamageCauser);

	virtual void StartDie();

	virtual bool IsAlive();
	
	float GetHpPercent() const;
};
