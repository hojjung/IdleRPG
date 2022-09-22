#include "Avatar/AvatarManager.h"

AvatarManager::AvatarManager()
{
	m_AryAvatars.Reserve(200);
	
	UAvatarData::GetAvatarTable->GetAllRows("",m_AryAvatars);

	m_AvatarCollection = new AvatarCollection();

	m_AvatarGacha = new AvatarGacha();

	m_AvatarInven = new AvatarInven();
	
	m_AvatarMerge = new AvatarMerge();
}

AvatarManager::~AvatarManager()
{
	m_AryAvatars.Reset();

	delete m_AvatarCollection;

	delete m_AvatarGacha;

	delete m_AvatarInven;
	
	delete m_AvatarMerge;
}

