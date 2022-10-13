// Fill out your copyright notice in the Description page of Project Settings.


#include "Pet/PetInven.h"

#include "MyGameInstance.h"
#include "Engine/AssetManager.h"
#include "Pet/PetAsset.h"
#include "Pet/PetData.h"

PetInven::PetInven()
{
	m_AryEquipkeys.Init(NAME_None, 4);
	m_AryEquipRows.Init(nullptr, 4);
}

PetInven::~PetInven()
{
	m_AryEquipkeys.Reset();
	m_AryEquipRows.Reset();
}

void PetInven::UpdatePet()
{
	//for unequip
	//for equip
	UPetAsset* Asset = nullptr;

	UAssetManager* Manager = UAssetManager::GetIfValid();
	
	int Iter = -1;
	
	while (++Iter < 4)
	{
		if(m_AryEquipkeys[Iter] == NAME_None)
		{
			continue;
		}
		Asset = Cast<UPetAsset>(Manager->GetPrimaryAssetObject(m_AryEquipRows[Iter]->m_EntityAsset));
		break;;
	}

	if(Asset != nullptr)
	{
		UMyGameInstance::Get->m_Player->SetPet(Asset);
	}
}

void PetInven::GetKeyEquip(int index, FName& key, const FPetDataRow** row)
{
	key = m_AryEquipkeys[index];
	*row = m_AryEquipRows[index];
}

void PetInven::SetKey(int index, const FName& key)
{
	m_AryEquipkeys[index] = key;

	const FPetDataRow* FoundRow = UPetData::GetPetData->FindRow<FPetDataRow>(key, "");

	m_AryEquipRows[index] = FoundRow;
	
	UpdatePet();
}

void PetInven::GetKeyVisualEquip(FName& key, const FPetDataRow** row)
{
	GetKeyEquip(0, key, row);
}

void PetInven::EquipPet(int index)
{
	if(m_PreviewID == NAME_None)
	{
		return;
	}

	SetKey(index, m_PreviewID);
}