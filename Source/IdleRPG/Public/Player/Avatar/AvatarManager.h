#pragma once

#include "CoreMinimal.h"
#include "AvatarCollection.h"
#include "AvatarData.h"
#include "AvatarGacha.h"
#include "AvatarInven.h"
#include "AvatarMerge.h"

class IDLERPG_API AvatarManager
{
public:
	AvatarManager();

	~AvatarManager();

public:
	AvatarCollection* m_AvatarCollection;

	AvatarGacha* m_AvatarGacha;

	AvatarInven* m_AvatarInven;

	AvatarMerge* m_AvatarMerge;
	
protected:
	TArray<const FAvatarRow*> m_AryAvatars;

public:
	FORCEINLINE const TArray<const FAvatarRow*>& GetAvatarDatas() const
	{
		return m_AryAvatars;
	}
};
