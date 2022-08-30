#include "GameAbilitySystem/MyStat.h"

FStatGroup FStatGroup::operator+(const FStatGroup& stat_group) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	+= stat_group.m_nAvoid;
	StatNew.m_nAccu		+= stat_group.m_nAccu;
	StatNew.m_MaxHp		+= stat_group.m_MaxHp;
	StatNew.m_Hp		+= stat_group.m_Hp;
	StatNew.m_Dmg		+= stat_group.m_Dmg;
	StatNew.m_AtkPerSec	+= stat_group.m_AtkPerSec;
	StatNew.m_MoveSpeed	+= stat_group.m_MoveSpeed;
	StatNew.m_CriPer	+= stat_group.m_CriPer;
	StatNew.m_CriDmg	+= stat_group.m_CriDmg;
	StatNew.m_DmgReduce += stat_group.m_DmgReduce;

	StatNew.m_nDef += stat_group.m_nDef;
	StatNew.m_Penet += stat_group.m_Penet;
	
	return StatNew;
}

FStatGroup FStatGroup::operator*(const FStatGroup& stat_group) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	*= stat_group.m_nAvoid;
	StatNew.m_nAccu		*= stat_group.m_nAccu;
	StatNew.m_MaxHp		*= stat_group.m_MaxHp;
	StatNew.m_Hp		*= stat_group.m_Hp;
	StatNew.m_Dmg		*= stat_group.m_Dmg;
	StatNew.m_AtkPerSec	*= stat_group.m_AtkPerSec;
	StatNew.m_MoveSpeed	*= stat_group.m_MoveSpeed;
	StatNew.m_CriPer	*= stat_group.m_CriPer;
	StatNew.m_CriDmg	*= stat_group.m_CriDmg;
	StatNew.m_DmgReduce *= stat_group.m_DmgReduce;

	StatNew.m_nDef *= stat_group.m_nDef;
	StatNew.m_Penet *= stat_group.m_Penet;
	
	return StatNew;
}

FStatGroup FStatGroup::operator-(const FStatGroup& stat_group) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	-= stat_group.m_nAvoid;
	StatNew.m_nAccu		-= stat_group.m_nAccu;
	StatNew.m_MaxHp		-= stat_group.m_MaxHp;
	StatNew.m_Hp		-= stat_group.m_Hp;
	StatNew.m_Dmg		-= stat_group.m_Dmg;
	StatNew.m_AtkPerSec	-= stat_group.m_AtkPerSec;
	StatNew.m_MoveSpeed	-= stat_group.m_MoveSpeed;
	StatNew.m_CriPer	-= stat_group.m_CriPer;
	StatNew.m_CriDmg	-= stat_group.m_CriDmg;
	StatNew.m_DmgReduce -= stat_group.m_DmgReduce;
	
	StatNew.m_nDef -= stat_group.m_nDef;
	StatNew.m_Penet -= stat_group.m_Penet;
	
	return StatNew;
}

FStatGroup FStatGroup::operator/(const FStatGroup& stat_group) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	/= stat_group.m_nAvoid;
	StatNew.m_nAccu		/= stat_group.m_nAccu;
	StatNew.m_MaxHp		/= stat_group.m_MaxHp;
	StatNew.m_Hp		/= stat_group.m_Hp;
	StatNew.m_Dmg		/= stat_group.m_Dmg;
	StatNew.m_AtkPerSec	/= stat_group.m_AtkPerSec;
	StatNew.m_MoveSpeed	/= stat_group.m_MoveSpeed;
	StatNew.m_CriPer	/= stat_group.m_CriPer;
	StatNew.m_CriDmg	/= stat_group.m_CriDmg;
	StatNew.m_DmgReduce /= stat_group.m_DmgReduce;

	StatNew.m_nDef /= stat_group.m_nDef;
	StatNew.m_Penet /= stat_group.m_Penet;
	
	return StatNew;
}
FStatGroup FStatGroup::operator*(float m) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	*= m;
	StatNew.m_nAccu		*= m;
	StatNew.m_MaxHp		*= m;
	StatNew.m_Hp		*= m;
	StatNew.m_Dmg		*= m;
	StatNew.m_AtkPerSec	*= m;
	StatNew.m_MoveSpeed	*= m;
	StatNew.m_CriPer	*= m;
	StatNew.m_CriDmg	*= m;
	StatNew.m_DmgReduce *= m;

	StatNew.m_nDef *= m;
	StatNew.m_Penet *= m;
	
	
	return StatNew;
}

FStatGroup FStatGroup::operator*(int m) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	*= m;
	StatNew.m_nAccu		*= m;
	StatNew.m_MaxHp		*= m;
	StatNew.m_Hp		*= m;
	StatNew.m_Dmg		*= m;
	StatNew.m_AtkPerSec	*= m;
	StatNew.m_MoveSpeed	*= m;
	StatNew.m_CriPer	*= m;
	StatNew.m_CriDmg	*= m;
	StatNew.m_DmgReduce *= m;

	StatNew.m_nDef *= m;
	StatNew.m_Penet *= m;
	
	return StatNew; 
}

FStatGroup& FStatGroup::operator+=(const FStatGroup& stat_group)
{
	m_nAvoid	+= stat_group.m_nAvoid;
	m_nAccu		+= stat_group.m_nAccu;
	m_MaxHp		+= stat_group.m_MaxHp;
	m_Hp		+= stat_group.m_Hp;
	m_Dmg		+= stat_group.m_Dmg;
	m_AtkPerSec	+= stat_group.m_AtkPerSec;
	m_MoveSpeed	+= stat_group.m_MoveSpeed;
	m_CriPer	+= stat_group.m_CriPer;
	m_CriDmg	+= stat_group.m_CriDmg;
	m_DmgReduce += stat_group.m_DmgReduce;

	m_nDef += stat_group.m_nDef;
	m_Penet += stat_group.m_Penet;

	return *this;
}

FStatGroup& FStatGroup::operator-=(const FStatGroup& stat_group)
{
	m_nAvoid	-= stat_group.m_nAvoid;
	m_nAccu		-= stat_group.m_nAccu;
	m_MaxHp		-= stat_group.m_MaxHp;
	m_Hp		-= stat_group.m_Hp;
	m_Dmg		-= stat_group.m_Dmg;
	m_AtkPerSec	-= stat_group.m_AtkPerSec;
	m_MoveSpeed	-= stat_group.m_MoveSpeed;
	m_CriPer	-= stat_group.m_CriPer;
	m_CriDmg	-= stat_group.m_CriDmg;
	m_DmgReduce -= stat_group.m_DmgReduce;
	
	m_nDef -= stat_group.m_nDef;
	m_Penet -= stat_group.m_Penet;

	
	return *this;
}
