#include "Monsters/Monster.h"
#include "Entity.h"
#include "MyLib.h"
#include "NavigationSystem.h"
#include "Monsters/MonsterPawn.h"

Monster::Monster(AMonsterPawn* pawn)
{
	m_Pawn = pawn;
	
	m_Gas = MakeShareable(new GAS(m_Pawn->GetUniqueID()));

	m_Pawn->SetGas(m_Gas);
	//
	m_fAlertTimer = -1.f;

	m_fIdleTimer = -1.f;

	m_fChaseFindTimer = -1.f;

	m_CurrentState = EFSM::Idle;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &Monster::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &Monster::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &Monster::OnCombat;
	//
	ResetStartPosition(m_Pawn->GetActorLocation());
}

Monster::~Monster()
{
	m_Gas.Reset();
}

void Monster::Update(float delta)
{
	m_fDeltaTime = delta;

	CheckSetState();

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();
}


void Monster::SetIdle()
{
	if (m_CurrentState == EFSM::Idle)
	{
		return;
	}
	m_CurrentState = EFSM::Idle;

	m_Pawn->SetFocusedTarget(nullptr);
		
	m_Pawn->StopMove();
}

bool Monster::CheckTargetRange(float rangeSqr)
{
	FVector Loc = m_Pawn->GetFocusedTarget()->GetActorLocation();
	Loc.Z = 0;

	FVector Loc2 = m_Pawn->GetActorLocation();
	Loc2.Z = 0;
	
	float DistSqr = FVector::DistSquared(Loc, Loc2);

	return DistSqr <= rangeSqr;
}

bool Monster::CheckAngle(float angleEuler)
{
	return UMyLib::CheckAngle(m_Pawn.Get(),m_Pawn->GetFocusedTarget(),angleEuler);
}

void Monster::CheckSetState()
{
	if (!m_Pawn->GetFocusedTarget())
	{
		SetIdle();
	}
	else
	{
		if (!CheckTargetRange(m_Pawn->GetAttackRangeSqr()))
		{
			m_CurrentState = EFSM::Chase;
		
			return;
		}	
		m_CurrentState = EFSM::Combat;
	}
}

void Monster::OnIdle()
{
	EPathFollowingStatus::Type Status = m_Pawn->GetPfComp()->GetStatus();

	if (m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_fDeltaTime;

		return;
	}

	FNavLocation Result;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_Pawn->GetWorld());

	if (EPathFollowingStatus::Idle == Status)
	{
		if (!NavSys->GetRandomPointInNavigableRadius(m_StartPoint, 400.f, Result))
		{
			return;
		}

		m_Pawn->MoveToLocation(Result);

		m_fIdleTimer = FMath::FRandRange(3.f, 7.f);
	}
}

void Monster::OnChase()
{
	m_Pawn->MoveToActor(m_Pawn->GetFocusedTarget(), m_Pawn->GetAttackRange());
}

void Monster::OnCombat()
{
	m_Pawn->HomingRotateToTarget(5, m_Pawn->GetFocusedTarget()->GetActorLocation());

	if(CheckAngle(60))
	{
		m_Pawn->TryAttack();
	}
}

void Monster::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}

AMonsterPawn* Monster::GetMonsterPawn()
{
	return m_Pawn.Get();
}

//
