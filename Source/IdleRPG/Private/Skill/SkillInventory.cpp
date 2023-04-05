#include "Skill/SkillInventory.h"
#include "Skill/SkillData.h"

SkillInventory::SkillInventory(const TArray<FSkillInven>& array)
{
	m_MapSkillInven.Reserve(50);

	USkillData::GetSkillData->ForeachRow<FSkillDataRow>("",[&](const FName& key, const FSkillDataRow& row)
	{
		FSkillInven SkillInvenData;

		SkillInvenData.m_SkillID = key;
		
		m_MapSkillInven.Add(key, SkillInvenData);
	});

	for(const FSkillInven& SkillFromSave : array)
	{
		FSkillInven* SkillInven = m_MapSkillInven.Find(SkillFromSave.m_SkillID);

		SkillInven->m_nCount = SkillFromSave.m_nCount;

		SkillInven->m_nLevel = SkillFromSave.m_nLevel;
	}

	m_ArySkillInst.Init(nullptr, 10);
}

SkillInventory::~SkillInventory()
{
	m_MapSkillInven.Reset();
}

float SkillInventory::GetSkillCd(const FName& id)
{
	return m_MapSkillInven[id].m_fCooldown;
}

void SkillInventory::LevelUpSkill(const FName& id)
{
	
}

void SkillInventory::EquipSkill(const FName& id, int index)
{
	FSkillInven* SkilInst = &m_MapSkillInven[id];
	
	m_ArySkillInst[index] = SkilInst;

	m_OnSkillEquipChanged.Broadcast(m_ArySkillInst);
}

void SkillInventory::UpdateCd(float deltaTime)
{
	for(FSkillInven* SkillInst : m_ArySkillInst)
	{
		if(SkillInst->m_SkillID == NAME_None)
		{
			continue;
		}
		
	}
}
