#include "Anims/MySingleAnimFSM.h"
#include "Actors/Pawns/MyBasePawn.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "DataAsset/EntityBaseAsset.h"

MySingleAnimFSM::MySingleAnimFSM(const UUnitEntityAsset* asset, AMyBasePawn* pawn)
{
	m_Owner = TStrongObjectPtr<AMyBasePawn>(pawn);

	m_Loco = asset->m_Loco.Get();

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

void MySingleAnimFSM::Update(float deltaTime)
{
	FVector Param (m_Owner->IsMoving() ? 1.f : 0.f , 0.f, 0.f);
	
	GetInst()->SetBlendSpaceInput(Param);
}

