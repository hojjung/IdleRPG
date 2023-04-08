#include "Skill/SkillData.h"

UDataTable* USkillData::GetSkillData = nullptr;

USkillData::USkillData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/02_DataTables/SkillTable.SkillTable'"));
	
	GetSkillData = Found.Object;
}

USkillBase::USkillBase()
{
	m_SkillInst = nullptr;
}

FText USkillBase::GetDescString(int lv)
{
	return m_TextDesc;//FText::Format(row.m_Desc, Cd)
}

void USkillBase::UseSkill()
{
	
}

void USkillBase::SetSkillInst(FSkillInven* inst)
{
	m_SkillInst = inst;
}