#include "Player/Avatar/AvatarData.h"
#include "Engine/DataTable.h"

UDataTable* UAvatarData::GetAvatarTable = nullptr;

UAvatarData::UAvatarData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/AvatarTable.AvatarTable'"));
	//
	GetAvatarTable = Found.Object;
}