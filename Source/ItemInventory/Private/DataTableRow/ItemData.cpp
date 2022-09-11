#include "DataTableRow/ItemData.h"

UDataTable* UItemData::GetItemTable = nullptr;

UItemData::UItemData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(
		TEXT("DataTable'/Game/DataTables/ItemTable.ItemTable'"));
	//
	GetItemTable = Found.Object;
}

TSoftObjectPtr<UTexture2D> FItemDataHandle::GetItemIcon() const
{
	if (IsNull())
	{
		return nullptr;
	}

	return GetRow<FItemDataRow>("")->m_Icon;
}


FItemDataRow::FItemDataRow()
{
	m_EquipStats = FStatGroup(0);
	m_EnchantStats = FStatGroup(0);
}
