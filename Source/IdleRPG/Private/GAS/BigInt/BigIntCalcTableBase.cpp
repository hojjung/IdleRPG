#include "GAS/BigInt/BigIntCalcTableBase.h"


UDataTable* UBigIntCalcTableBase::GetBigIntTable = nullptr;


UBigIntCalcTableBase::UBigIntCalcTableBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/BigintTable.BigintTable'"));
	//
	GetBigIntTable = Found.Object;
}
