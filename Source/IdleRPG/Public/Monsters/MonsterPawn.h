// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Player/CombatPawn.h"
#include "MonsterPawn.generated.h"

class UWidgetPawnInfoComp;
class Monster;
/**
 * 
 */
UCLASS()
class IDLERPG_API AMonsterPawn : public ACombatPawn
{
	GENERATED_BODY()

public:
	AMonsterPawn(const FObjectInitializer& obj);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UWidgetPawnInfoComp* m_PawnInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* m_SoundComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* m_HitParticle;

	float m_fHitAnimCD;
	
public:
	virtual bool UseBoidMove() override;

	virtual void SetGas(TSharedPtr<GAS> newGas) override;
	
	virtual void SetEntity(const UUnitAsset* asset) override;
private:
	
	void PlayTookHitMontage();

	virtual void Tick(float DeltaSeconds) override;
	
	void OnHpChanged();

	void OnTookDamage(BigInt dmg);
};
