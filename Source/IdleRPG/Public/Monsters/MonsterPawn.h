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
	UAudioComponent* m_CoinSoundComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* m_HitParticle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* m_CoinParticle;
	UPROPERTY()
	USoundBase* m_DefaultHitSound;
	UPROPERTY()
	USoundBase* m_CriHitSound;
private:
	float m_fDeathAnimDurationMax;
	
	float m_fDeathAnimDurationTimer;
	
	float m_fHitAnimCD;

	FFloatCurve m_CurveDeathAnim;

	FTimerHandle m_DeathAnimTimer;

	FTimerHandle m_StunTimer;

public:
	virtual bool UseBoidMove() override;

	virtual void SetGas(TSharedPtr<GAS> newGas) override;
	
	virtual void SetEntity(const UUnitAsset* asset) override;
	
private:
	virtual void Tick(float DeltaSeconds) override;
	
	void OnHpChanged();
	
	void PlayTookHitMontage();

	void PlayHitFlash();

	void PlayHitEffect();

	void PlayCoinEffect();

	void PlayHittenSound(EDamagePopup pop);

	void OnTookDamage(BigInt dmg, EDamagePopup pop);

	virtual void OnDead() override;

	virtual void PlayDeathAnim() override;

	void CreateSetDeathCurve(float fullLength);

	void SetDeathEffectMaterial(float deltaTime);

	void StartDeathEffectMaterial(float duration);
};
