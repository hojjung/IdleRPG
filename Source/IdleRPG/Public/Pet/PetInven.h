// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Avatar/AvatarInven.h"

struct FPetDataRow;
/**
 * 
 */
class IDLERPG_API PetInven: public PreviewInven
{
public:
	PetInven();
	
	virtual  ~PetInven() override;

protected:
	TArray<FName> m_AryEquipkeys;
	
	TArray<const FPetDataRow*> m_AryEquipRows;

protected:
	void UpdatePet();

public:
	void GetKeyEquip(int index, FName& key, const FPetDataRow** row);

	void SetKey(int index, const FName& key);

	void GetKeyVisualEquip(FName& key, const FPetDataRow** row);
	
	void EquipPet(int index);
};
