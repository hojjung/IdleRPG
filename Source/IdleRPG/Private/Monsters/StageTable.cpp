#include "Monsters/StageTable.h"

UDataTable* UStageTable::GetDefaultStage = nullptr;

UStageTable::UStageTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/02_DataTables/StageTable.StageTable'"));

	GetDefaultStage = FoundTable01.Object;
}

TArray<FPrimaryAssetId> FStageRow::GetStageUnits(int lv) const
{
	int Zone = lv % 20;

	Zone /= 2;

	TArray<FPrimaryAssetId> NewAry;
	NewAry.Add(m_AryUnits[Zone]);

	int Iter = Zone;

	int Count = 0;

	while (--Iter >= 0 && Count < 2)
	{
		NewAry.Add(m_AryUnits[Iter]);
		
		Count++;
	}
	
	return NewAry;
}
