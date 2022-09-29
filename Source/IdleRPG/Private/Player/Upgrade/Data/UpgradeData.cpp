#include "Player/Upgrade/Data/UpgradeData.h"

UDataTable* UUpgradeData::GetUpgradeTable = nullptr;

UUpgradeData::UUpgradeData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/UpgradeTable.UpgradeTable'"));
	//
	GetUpgradeTable = Found.Object;
}