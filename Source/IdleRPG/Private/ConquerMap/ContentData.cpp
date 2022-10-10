
#include "ConquerMap/ContentData.h"

UDataTable* UContentData::GetContentData = nullptr;

UContentData::UContentData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/ContentTable.ContentTable'"));
	
	GetContentData = Found.Object;
}
