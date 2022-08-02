#include "Player/PlayerFSM.h"

#include "MyLib.h"
#include "Player/MyPlayerPawn.h"

PlayerFSM::PlayerFSM(AMyPlayerPawn* pl)
{
	m_Pl = pl;

	m_CurrentState = EFSM::Idle;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &PlayerFSM::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &PlayerFSM::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &PlayerFSM::OnCombat;
	//
	ResetStartPosition(m_Pl.Get()->GetActorLocation());
}

PlayerFSM::~PlayerFSM()
{
}

void PlayerFSM::Update(float delta)
{
	CheckSetState();

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();
}

void PlayerFSM::CheckSetState()
{
	ACombatPawn* Focused = m_Pl->GetFocusedTarget();
	
	if(m_Pl->IsInputMoving() || !Focused)
	{
		m_CurrentState = EFSM::Idle;
		return;
	}

	if (!Focused->IsAlive())
	{
		m_CurrentState = EFSM::Idle;
		return;
	}
	
	ACombatPawn* NPCPawn = Cast<ACombatPawn>( m_Pl->GetFocusedTarget());
	
	float Range = m_Pl->GetAttackRangeSqr();

	if (m_Pl->IsRangeMode())
	{
		if(!m_Pl->LineOfSightTo(NPCPawn))
		{
			Range = 100;	
		}
	}

	if (!CheckTargetRange(Range))
	{
		m_CurrentState = EFSM::Chase;
		return;
	}	

	m_CurrentState = EFSM::Combat;
}

void PlayerFSM::OnIdle()
{
	
}

void PlayerFSM::OnChase()
{
	m_Pl->ChaseTarget();
}

void PlayerFSM::OnCombat()
{
	m_Pl->HomingRotateToTarget();

	if(CheckAngle(60))
	{
		m_Pl->TryAttack();
	}
}

bool PlayerFSM::CheckTargetRange(float rangeSqr)
{
	FVector Loc = m_Pl->GetFocusedTarget()->GetActorLocation();
	
	float DistSqr = FVector::DistSquared(m_Pl->GetActorLocation(), Loc);

	return DistSqr <= rangeSqr;
}

bool PlayerFSM::CheckAngle(float angleEuler)
{
	return UMyLib::CheckAngle(m_Pl.Get(),m_Pl->GetFocusedTarget(),angleEuler);
}

void PlayerFSM::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}
