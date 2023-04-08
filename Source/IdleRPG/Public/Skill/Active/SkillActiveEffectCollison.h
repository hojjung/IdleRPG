// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "Skill/Active/SkillActiveBase.h"
#include "SkillActiveEffectCollison.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API USkillActiveEffectCollison : public USkillActiveBase, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
	
public:
	USkillActiveEffectCollison();
	
protected:
	float m_fRadius;
	
	float m_fStartDamage;

	float m_fLevelUpDamage;

	float m_fDelay;

	EDmgType m_DmgType;
	
	TArray<AMonsterPawn*> m_AryMonsters;

	UPROPERTY()
	UNiagaraSystem* m_Effect;
	
protected:
	virtual FText GetDescString(int lv) override;
	
	float GetLevelPerDmg();

	float GetLevelPerDmg(int lv);
	
	virtual void UseSkill() override;

	virtual void BeginDestroy() override;

	void OnTrace(FVector pos);

	virtual void ReceiveParticleData(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem) override;
};
