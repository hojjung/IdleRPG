#include "Player/Upgrade/Data/ClassUpData.h"

UDataTable* UClassUpData::GetUpgradeTable = nullptr;

UClassUpData::UClassUpData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/ClassTable.ClassTable'"));
	//
	GetUpgradeTable = Found.Object;
}