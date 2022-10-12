// Fill out your copyright notice in the Description page of Project Settings.


#include "Pet/PetData.h"

UDataTable* UPetData::GetPetData = nullptr;

UPetData::UPetData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/PetTable.PetTable'"));
	
	GetPetData = Found.Object;	
}
