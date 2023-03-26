#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"


/**
 * 레벨과 스킬 조각 등 관리
 */
class IDLERPG_API SkillInventory
{
public:
	SkillInventory();
	
	SkillInventory(const TArray<FSkillInven>& ary);
	
	~SkillInventory();

protected:
	TMap<FName, FSkillInven> m_MapSkillInven;

public:
	FORCEINLINE const TMap<FName, FSkillInven>& GetMapSkillInven() const
	{
		return m_MapSkillInven;
	}
};
