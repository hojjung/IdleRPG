#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"


/**
 * 레벨과 스킬 조각 등 관리
 */

class IDLERPG_API SkillInventory
{
public:
	SkillInventory() {};
	
	SkillInventory(const TArray<FSkillInven>& array);
	
	~SkillInventory();
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEquipChanged, const TArray<FSkillInven*>&);

	FOnSkillEquipChanged m_OnSkillEquipChanged;

protected:
	TMap<FName,FSkillInven> m_MapSkillInven;

	TArray<FSkillInven*> m_ArySkillInst;

protected:
	float GetSkillCd(const FName& id);
	

public:
	FORCEINLINE const TMap<FName, FSkillInven>& GetMapSkillInven() const
	{
		return m_MapSkillInven;
	}

	FORCEINLINE const TArray<FSkillInven*>& GetSkillInstInven() const
	{
		return m_ArySkillInst;
	}

	void LevelUpSkill(const FName& id);
	
	void EquipSkill(const FName& id, int index);

	void UpdateCd(float deltaTime);
};
