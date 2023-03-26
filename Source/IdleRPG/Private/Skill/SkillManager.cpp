#include "Skill/SkillManager.h"

void SkillManager::SetSkill(const TArray<FSkillInven>& ary)
{
	m_SkillInven = MakeShareable(new SkillInventory(ary));
	
	m_SkillEquip = MakeShareable(new SkillEquip());
}

SkillManager::SkillManager()
{
	
}

SkillManager::~SkillManager()
{
	m_SkillInven.Reset();

	m_SkillEquip.Reset();
}

