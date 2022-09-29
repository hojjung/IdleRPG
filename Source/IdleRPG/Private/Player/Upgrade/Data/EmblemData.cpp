#include "Player/Upgrade/Data/EmblemData.h"

UDataTable* UEmblemData::GetUpgradeTable = nullptr;

UEmblemData::UEmblemData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/EmblemTable.EmblemTable'"));
	//
	GetUpgradeTable = Found.Object;
}