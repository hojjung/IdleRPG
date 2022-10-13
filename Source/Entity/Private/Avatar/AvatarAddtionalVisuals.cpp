#include "Avatar/AvatarAddtionalVisuals.h"
#include "NavigationSystem.h"

void UAvatarAddtionalVisuals::Init(USkeletalMeshComponent* owner)
{
	m_fAttackDur = -1;
	m_Owner = owner;
	m_AryCapes.Reset(10);
}

void UAvatarAddtionalVisuals::Hide()
{
	for(UMyCapeComponent* Cape : m_AryCapes)
	{
		Cape->SetHiddenInGame(true);
		Cape->SetVisibility(false);
		Cape->DestroyComponent();
	}
}

void UAvatarAddtionalVisuals::SpawnAttachment(FName id, UStaticMesh* mesh)
{
	AActor* Owner = m_Owner.Get()->GetOwner();
	
	UMyCapeComponent* Cape =  NewObject<UMyCapeComponent>(Owner, "CapeNew",RF_Transient);
	
	Cape->CreationMethod = EComponentCreationMethod::Instance;
	
	FAttachmentTransformRules Rule (EAttachmentRule::SnapToTarget, true);
	Cape->AttachToComponent(m_Owner.Get(), Rule, id);
	
	Owner->AddInstanceComponent(Cape);
	Owner->AddOwnedComponent(Cape);
	Cape->RegisterComponent();
	
	Cape->Init(mesh);
	
	m_AryCapes.Add(Cape);
}

void UAvatarAddtionalVisuals::TickWind(bool is_moving, float deltaTime)
{
	m_fAttackDur -= deltaTime;

	if(m_fAttackDur > 0)
	{
		for(UMyCapeComponent* Cape : m_AryCapes)
		{
			Cape->SetWindPower(30);
		}
		return;
	}
	
	if(is_moving)
	{
		for(UMyCapeComponent* Cape : m_AryCapes)
		{
			Cape->SetWindPower(20);
		}
	}
	else
	{
		for(UMyCapeComponent* Cape : m_AryCapes)
		{
			Cape->SetWindPower(10);
		}
	}
}

void UAvatarAddtionalVisuals::SetAttacking(float dur)
{
	m_fAttackDur = dur;
}
