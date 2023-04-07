#include "Skill/SkillInventory.h"

#include "Manager/MyGameInstance.h"
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
	
	m_MapSkillInstance.Reserve(10);
}

SkillInventory::~SkillInventory()
{
	m_ArySkillInst.Reset();
	m_MapSkillInven.Reset();
	m_MapSkillInstance.Reset();
}

float SkillInventory::GetSkillCd(const FName& id)
{
	return m_MapSkillInven[id].m_fCooldownRemain;
}

bool SkillInventory::IsCooldownReady(const FSkillInven* skill)
{
	return skill->m_fCooldownRemain <= 0;
}

void SkillInventory::SetCooldown(FSkillInven* skill)
{
	const float Cd = USkillData::GetSkillData->FindRow<FSkillDataRow>(skill->m_SkillID,"")->m_fCooltime;

	skill->m_fCooldownRemain = Cd;
}

void SkillInventory::LevelUpSkill(const FName& id)
{
	
}

void SkillInventory::UnEquipSkill(int index, bool update)
{
	FSkillInven* Skill = m_ArySkillInst[index];

	if(!Skill)
	{
		return;
	}

	m_ArySkillInst[index] = nullptr;
	
	m_MapSkillInstance.Remove(Skill);

	if(update)
	{
		m_OnSkillEquipChanged.Broadcast(m_ArySkillInst);
	}
}

void SkillInventory::EquipSkill(const FName& id, int index)
{
	FSkillInven* SkilInst = &m_MapSkillInven[id];

	if(m_ArySkillInst[index])
	{
		UnEquipSkill(index);
	}
	
	m_ArySkillInst[index] = SkilInst;

	TSubclassOf<USkillBase> SkillClass = USkillData::GetSkillData->FindRow<FSkillDataRow>(id,"")->m_ClassSkillBase;

	USkillBase* SkillInst = NewObject<USkillBase>(UMyGameInstance::Get, SkillClass);

	SkillInst->SetSkillInst(m_ArySkillInst[index]);

	m_MapSkillInstance.Add(SkilInst,TStrongObjectPtr<USkillBase>(SkillInst));

	m_OnSkillEquipChanged.Broadcast(m_ArySkillInst);
}

void SkillInventory::UpdateCd(float deltaTime)
{
	for(FSkillInven* SkillInst : m_ArySkillInst)
	{
		if(!SkillInst || SkillInst->m_SkillID == NAME_None)
		{
			continue;
		}

		SkillInst->m_fCooldownRemain -= deltaTime;
	}
}

void SkillInventory::UseSkill(int ndx)
{
	FSkillInven* Skill = m_ArySkillInst[ndx];
	
	if(!IsCooldownReady(Skill))
	{
		return;
	}
	
	m_MapSkillInstance[Skill]->UseSkill();

	SetCooldown(Skill);
}