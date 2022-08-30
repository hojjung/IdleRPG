#include "DataTableRow/AvatarData.h"

UDataTable* UAvatarData::GetAvatarTable = nullptr;

UAvatarData::UAvatarData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Build/BuildDataTable.BuildDataTable'"));
	//
	GetAvatarTable = Found.Object;
}