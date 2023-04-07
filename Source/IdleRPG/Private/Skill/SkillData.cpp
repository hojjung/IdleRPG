#include "Skill/SkillData.h"

UDataTable* USkillData::GetSkillData = nullptr;

USkillData::USkillData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/SkillTable.SkillTable'"));
	
	GetSkillData = Found.Object;
}
