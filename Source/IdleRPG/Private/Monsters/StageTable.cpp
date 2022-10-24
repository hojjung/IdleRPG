#include "Monsters/StageTable.h"

UDataTable* UStageTable::GetDefaultStage = nullptr;

UStageTable::UStageTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/02_DataTables/StageTable.StageTable'"));

	GetDefaultStage = FoundTable01.Object;
}
