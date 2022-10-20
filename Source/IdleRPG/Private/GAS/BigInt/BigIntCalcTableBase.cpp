#include "GAS/BigInt/BigIntCalcTableBase.h"


UDataTable* UBigIntCalcTableBase::GetGoldTable = nullptr;

UDataTable* UBigIntCalcTableBase::GetPlayerUpgradeTable = nullptr;

UDataTable* UBigIntCalcTableBase::GetLevelUpTable = nullptr;

UDataTable* UBigIntCalcTableBase::GetExpTable = nullptr;


UBigIntCalcTableBase::UBigIntCalcTableBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/BigInt/GoldTable.GoldTable'"));
	//
	GetGoldTable = Found.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> Found2(TEXT("DataTable'/Game/02_DataTables/BigInt/UpgradeTable.UpgradeTable'"));
	//
	GetPlayerUpgradeTable = Found2.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> Found3(TEXT("DataTable'/Game/02_DataTables/BigInt/LevelUpTable.LevelUpTable'"));

	GetLevelUpTable = Found3.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> Found4(TEXT("DataTable'/Game/02_DataTables/BigInt/ExpTable.ExpTable'"));

	GetExpTable = Found4.Object;
}
