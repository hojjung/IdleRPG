#include "Monsters/StageTable.h"

UDataTable* UStageTable::GetData = nullptr;

UStageTable::UStageTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/DataTables/StageTable.StageTable'"));

	GetData = FoundTable01.Object;
}
