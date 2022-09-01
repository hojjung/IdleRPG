#pragma once

#include "CoreMinimal.h"
#include "DataTableRow/AvatarData.h"

class ENTITY_API AvatarManager
{
public:
	AvatarManager();
	
	~AvatarManager();

protected:
	TArray<const FAvatarRow*> m_AryAvatars;


public:
	FORCEINLINE const TArray<const FAvatarRow*>& GetAvatarDatas() const
	{
		return m_AryAvatars;
	}
};
