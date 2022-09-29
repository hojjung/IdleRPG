#include "Player/Upgrade/Data/LevelUpData.h"

UDataTable* ULevelUpData::GetUpgradeTable = nullptr;

ULevelUpData::ULevelUpData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/LevelUpTable.LevelUpTable'"));
	//
	GetUpgradeTable = Found.Object;
}