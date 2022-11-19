#include "Anims/MyAnimInstance.h"
#include "Entity.h"

FMyAnimInstanceProxy::FMyAnimInstanceProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance)
{
	m_bIsMoving = false;
}

void FMyAnimInstanceProxy::Update(float DeltaSeconds)
{
	AMyBasePawn* Pawn = GetSkelMeshComponent()->GetOwner<AMyBasePawn>();

	if(Pawn == nullptr)
	{
		return;
	}
	m_bIsMoving = Pawn->IsMoving();
}
