#include "Avatar/AvatarManager.h"

AvatarManager::AvatarManager()
{
	m_AryAvatars.Reserve(200);
	
	UAvatarData::GetAvatarTable->GetAllRows("",m_AryAvatars);
	
}

AvatarManager::~AvatarManager()
{
	m_AryAvatars.Reset();
	
}
