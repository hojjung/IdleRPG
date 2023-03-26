#include "Skill/SkillInventory.h"
#include "Skill/SkillData.h"

SkillInventory::SkillInventory()
{
	
}

SkillInventory::SkillInventory(const TArray<FSkillInven>& ary)
{
	m_MapSkillInven.Reserve(50);

	USkillData::GetSkillData->ForeachRow<FSkillDataRow>("",[&](const FName& key, const FSkillDataRow& row)
	{
		FSkillInven SkillInvenData;

		SkillInvenData.m_SkillID = key;
		
		m_MapSkillInven.Add(key, SkillInvenData);
	});

	for(const FSkillInven& SkillFromSave : ary)
	{
		FSkillInven* SkillInven = m_MapSkillInven.Find(SkillFromSave.m_SkillID);

		SkillInven->m_nCount = SkillFromSave.m_nCount;

		SkillInven->m_nLevel = SkillFromSave.m_nLevel;
	}
}

SkillInventory::~SkillInventory()
{
	m_MapSkillInven.Reset();
}
