#include "Monsters/StageTable.h"

UDataTable* UStageTable::GetData = nullptr;

UStageTable::UStageTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/02_DataTables/StageTable.StageTable'"));

	GetData = FoundTable01.Object;
}
