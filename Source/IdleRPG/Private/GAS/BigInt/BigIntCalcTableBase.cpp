#include "GAS/BigInt/BigIntCalcTableBase.h"


UDataTable* UBigIntCalcTableBase::GetGoldTable = nullptr;


UBigIntCalcTableBase::UBigIntCalcTableBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/AvatarTable.AvatarTable'"));
	//
	GetGoldTable = Found.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/AvatarTable.AvatarTable'"));
	//
	GetPlayerUpgradeTable = Found.Object;
}
