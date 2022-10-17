#include "Player/PlayerSensor.h"
#include "MyGameInstance.h"
#include "MyLib.h"
#include "Monsters/MonsterPawn.h"
#include "Player/MyPlayerPawn.h"

PlayerSensor::PlayerSensor(AMyPlayerPawn* pl): m_SensingTimer(0)
{
	m_Pl = pl;
}

PlayerSensor::~PlayerSensor()
{
	
}

AMonsterPawn* PlayerSensor::GetNearTarget(float SearchRange)
{
	float MaxRange = MAX_flt;
	
	FVector Loc = m_Pl->GetActorLocation();
	
	m_AryMobs.Reset(10);

	UMyGameInstance::Get->m_SpawnManager->GetNearNpcs<AMonsterPawn>(m_Pl.Get(), m_AryMobs, SearchRange);

	AMonsterPawn* Target = nullptr;
	
	for (AMonsterPawn* Focus : m_AryMobs)
	{
		if (!Focus->IsAlive())
		{
			continue;
		}

		float NavLen = 0.f;

		FVector DestLoc = Focus->GetActorLocation();

		float Cost = 0.f;
		
		ENavigationQueryResult::Type ResultT = UMyLib::GetNavSys()->GetPathLengthAndCost(Loc, DestLoc, NavLen, Cost);

		if (Cost > MaxRange)
		{
			continue;
		}

		MaxRange = Cost;

		Target = Focus;
	}
	return Target;
}

void PlayerSensor::Update(float delta)
{
	if(m_Pl->GetFocusedTarget())
	{
		return;
	}
	m_SensingTimer += delta;

	if(m_SensingTimer < 0.3f)
	{
		return;
	}
	m_SensingTimer = 0.f;
	
	AMonsterPawn* Target = Target = GetNearTarget(9000);

	if(Target)
	{
		m_Pl->SetFocusedTarget(Target);
		return ;
	}
	m_Pl->SetFocusedTarget(nullptr);
}
