// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableRow/ColorData.h"


UDataTable* UColorData::GetColorTable = nullptr;

UColorData::UColorData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/DataTables/ColorTable.ColorTable'"));
	//
	GetColorTable = Found.Object;
}
