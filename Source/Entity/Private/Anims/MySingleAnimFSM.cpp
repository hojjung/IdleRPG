#include "Anims/MySingleAnimFSM.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "DataAsset/EntityBaseAsset.h"

MySingleAnimFSM::MySingleAnimFSM(const UUnitEntityAsset* asset, AMyBasePawn* pawn)
{
	m_Owner = TStrongObjectPtr<AMyBasePawn>(pawn);

	m_Loco = asset->m_Loco;

	m_Atk = asset->m_BaseAttackAnim;

	m_Owner->GetSkMesh()->PlayAnimation(m_Loco.Get(), true);
}

MySingleAnimFSM::~MySingleAnimFSM()
{
	m_Loco.Reset();

	m_Owner.Reset();
}

UAnimSingleNodeInstance* MySingleAnimFSM::GetInst()
{
	return  m_Owner->GetSkMesh()->GetSingleNodeInstance();
}

bool MySingleAnimFSM::IsSlotPlaying(float delta)
{
	if (m_fSlotDur <= -1.f)
	{
		return false;
	}
	else
	{
		m_fSlotTimer += delta;

		if (m_fSlotTimer >= m_fSlotDur)
		{
			GetInst()->SetPlayRate(1.f);

			m_fSlotDur = -1.f;

			m_Owner->GetSkMesh()->PlayAnimation(m_Loco.Get(), true);

			return false;
		}
	}
	return true;
}

void MySingleAnimFSM::Update(float deltaTime)
{
	if(IsSlotPlaying(deltaTime))
	{
		return;
	}
	
	FVector Param (m_Owner->IsMoving() ? 1.f : 0.f , 0.f, 0.f);
	
	GetInst()->SetBlendSpaceInput(Param);
}

void MySingleAnimFSM::PlaySlotAnim(UAnimSequenceBase* anim, float rate)
{
	m_Owner->GetSkMesh()->PlayAnimation(anim, false);
	
	//GetInst()->SetPlayRate(rate);

	float Len = GetInst()->GetLength();

	m_fSlotDur = (Len - 0.15f);// / rate;

	m_fSlotTimer = 0.f;
}

void MySingleAnimFSM::Attack()
{
	PlaySlotAnim(m_Atk.Get(),2.f);
}
