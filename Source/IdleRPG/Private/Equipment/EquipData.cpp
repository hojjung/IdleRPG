#include "Equipment/EquipData.h"

UDataTable* UEquipData::GetWeaponData = nullptr;
UDataTable* UEquipData::GetHelmetData = nullptr;
UDataTable* UEquipData::GetTorsoData = nullptr;
UDataTable* UEquipData::GetGloveData = nullptr;
UDataTable* UEquipData::GetLegData = nullptr;
UDataTable* UEquipData::GetFeetData = nullptr;

UEquipData::UEquipData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable01(TEXT("DataTable'/Game/02_DataTables/Equipments/WeaponData.WeaponData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable02(TEXT("DataTable'/Game/02_DataTables/Equipments/HelmetData.HelmetData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable03(TEXT("DataTable'/Game/02_DataTables/Equipments/TorsoData.TorsoData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable04(TEXT("DataTable'/Game/02_DataTables/Equipments/GloveData.GloveData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable05(TEXT("DataTable'/Game/02_DataTables/Equipments/LegData.LegData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable06(TEXT("DataTable'/Game/02_DataTables/Equipments/FeetData.FeetData'"));
	
	GetWeaponData = FoundTable01.Object;
	GetHelmetData = FoundTable02.Object;
	GetTorsoData = FoundTable03.Object;
	GetGloveData = FoundTable04.Object;
	GetLegData = FoundTable05.Object;
	GetFeetData = FoundTable06.Object;
}
