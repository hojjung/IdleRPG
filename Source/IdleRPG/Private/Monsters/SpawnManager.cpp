#include "Monsters/SpawnManager.h"

SpawnManager::SpawnManager()
{
	
}

SpawnManager::~SpawnManager()
{
	for(auto Mob : m_AryMonsters)
	{
		Mob.Reset();
	}
	m_AryMonsters.Reset();
}

void SpawnManager::Update(float delta)
{
	for(auto Mob : m_AryMonsters)
	{
		Mob.Get()->Update(delta);
	}
}
