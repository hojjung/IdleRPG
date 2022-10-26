#include "Manager/GameMode/MyGameModeBase.h"
#include "Monsters/SpawnManager.h"

MyGameModeBase::MyGameModeBase(): m_nLevel(-1)
{
}

MyGameModeBase::~MyGameModeBase()
{
	m_nLevel = -1;
	m_SpawnManager.Reset();
}

void MyGameModeBase::SetLevel(int l)
{
	m_nLevel = l;

	m_LevelChanged.Broadcast();
}

FText MyGameModeBase::GetStageName()
{
	return FText::FromString(TEXT("DefaultStageName"));
}

void MyGameModeBase::Update(float delta_time)
{
	m_SpawnManager->Update(delta_time);
	
}
