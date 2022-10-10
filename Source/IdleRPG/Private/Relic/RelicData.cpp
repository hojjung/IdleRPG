// Fill out your copyright notice in the Description page of Project Settings.


#include "Relic/RelicData.h"
UDataTable* URelicData::GetRelicData = nullptr;
URelicData::URelicData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/RelicTable.RelicTable'"));
	
	GetRelicData = Found.Object;
}
