#include "Player/Avatar/AvatarManager.h"

AvatarManager::AvatarManager()
{
	m_AvatarCollection = new AvatarCollection();

	m_AvatarGacha = new AvatarGacha();

	m_AvatarInven = new AvatarInven();
	
	m_AvatarMerge = new AvatarMerge();
}

AvatarManager::~AvatarManager()
{
	delete m_AvatarCollection;

	delete m_AvatarGacha;

	delete m_AvatarInven;
	
	delete m_AvatarMerge;
}

