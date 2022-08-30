#include "DataTableRow/AvatarData.h"

UDataTable* UAvatarData::GetAvatarTable = nullptr;

UAvatarData::UAvatarData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/DataTables/AvatarTable.AvatarTable'"));
	//
	GetAvatarTable = Found.Object;
}