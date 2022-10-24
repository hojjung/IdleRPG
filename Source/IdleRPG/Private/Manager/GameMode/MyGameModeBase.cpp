#include "Manager/GameMode/MyGameModeBase.h"
#include "Monsters/SpawnManager.h"

MyGameModeBase::MyGameModeBase(int lv)
{
	m_nLevel = lv;
}

MyGameModeBase::~MyGameModeBase()
{
	m_nLevel = -1;
	m_SpawnManager.Reset();
}

FText MyGameModeBase::GetDefaultStageName(int level)
{
	return FText::FromString(TEXT("DefaultStageName"));
}

void MyGameModeBase::Update(float delta_time)
{
	m_SpawnManager->Update(delta_time);
	
}
