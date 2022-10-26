// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "GAS/GAS.h"
#include "UObject/StrongObjectPtr.h"
#include "CombatPawn.generated.h"

UENUM(BlueprintType)
enum class EDmgType : uint8
{
	DmgPhys
	
};

UENUM(Blueprintable)
enum class EDamagePopup: uint8
{
	Normal,
	Critcal,
	Critcal2,
	SwordBomb,
	Miss,
	Length
};

UCLASS()
class IDLERPG_API ACombatPawn : public AMyBasePawn
{
	GENERATED_BODY()

public:
	ACombatPawn(const FObjectInitializer& objInit);

protected:
	TWeakPtr<GAS> m_Gas;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_ShadowMeshComp;
	
	TWeakObjectPtr<ACombatPawn> m_Target;

	FTimerHandle m_DeathAnimTimer;
	
	float m_fAttackCD;

	float m_fAtkRange;

	float m_fAtkRangeSqr;

	TMap<TStrongObjectPtr<UObject>, TStrongObjectPtr<UActorComponent>> m_MapComp;
	
	FVector m_SpawnPos;

	ECollisionEnabled::Type m_InitColl;
	
protected:
	virtual float PlayBaseAttackAnim(float rate);

	void SetAtkRange(float v);

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnDead();

	virtual void PlayDeathAnim();

	virtual void OnDeathAnimEnd();

	virtual void OnReviveAnimEnd();
	
public:
	virtual void Revive();
	
	virtual void OnNotifyTrigger(const FName& id);
	
	virtual void SetGas(TSharedPtr<GAS> newGas);

	GAS* GetGas();
	
	virtual void SetEntity(const UUnitAsset* asset) override;
	
	void SetFocusedTarget(ACombatPawn* pawn);
	
	ACombatPawn* GetFocusedTarget();
	
	float TryAttack(float playRate = 1.f);

	
	virtual bool IsRangeMode() {return true;}
	
	float GetAttackRange();

	float GetAttackRangeSqr();

	EPathFollowingRequestResult::Type ChaseTarget();

	virtual void MyTakeDamage(ACombatPawn* DamageCauser, EDmgType dmgType);

	virtual bool IsAlive();
	
	float GetHpPercent() const;
public:
	void AddComp(UObject* key, UActorComponent* get);
	
	UActorComponent* FindComp(UObject* key);

};
