#include "Monsters/Monster.h"
#include "Monsters/MonsterPawn.h"

Monster::Monster(AMonsterPawn* pawn, UUnitEntityAsset* asset)
{
	m_Pawn = pawn;
}

Monster::~Monster()
{
	
}

void Monster::Update(float delta)
{
	
}

float Monster::GetHpPercent()
{
	float Per = 0.f;

	if(m_fMaxHp <= 0.f)
	{
		return Per; 
	}

	Per = m_fHp / m_fMaxHp;

	return Per;
}
