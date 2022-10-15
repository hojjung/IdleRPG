#include "Monsters/MonsterPawn.h"
#include "Components/AudioComponent.h"
#include "Monsters/Monster.h"
#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfoComp.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/MyFlockSteering.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& obj): Super(obj.SetDefaultSubobjectClass<UMyFlockSteering>(TEXT("m_Movement")))
{
	m_SoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio01"));
	m_SoundComp->SetupAttachment(RootComponent);
	m_SoundComp->SetAutoActivate(false);

	m_HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleHit02"));
	m_HitParticle->SetupAttachment(RootComponent);

	m_HitParticle->SetRelativeLocation(FVector(0, 0, 0.f));
	m_HitParticle->SetAutoActivate(false);
	m_HitParticle->SetRelativeScale3D(FVector(0.5f));
	m_HitParticle->Deactivate();
	//PawnInfoWidget
	m_PawnInfo = CreateDefaultSubobject<UWidgetPawnInfoComp>(TEXT("PawnInfoWidget"));
	m_PawnInfo->SetupAttachment(RootComponent);
	m_PawnInfo->SetVisibility(false);
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundHpBar(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/Comp/WB_PawnInfo.WB_PawnInfo_C'"));
	m_PawnInfo->SetWidgetClass(FoundHpBar.Class);
	//m_PawnInfo->SetDrawSize(FVector2D(150.f, 22.f));
	m_PawnInfo->SetDrawAtDesiredSize(true);
	FVector2D Pivot(0.5f, 0.5f);
	m_PawnInfo->SetPivot(Pivot);
	m_PawnInfo->SetWidgetSpace(EWidgetSpace::Screen);
	m_PawnInfo->SetRelativeLocation(FVector(0, 0, 130));
	m_PawnInfo->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundHitEffect(
TEXT("ParticleSystem'/Game/03_VisualEffect/P_Hit.P_Hit'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FoundHitSound(
		TEXT("SoundWave'/Game/Sound/Use/Fantasy_Game_Weapon_Impact.Fantasy_Game_Weapon_Impact'"));

	m_HitParticle->SetTemplate(FoundHitEffect.Object);

	m_SoundComp->SetSound(FoundHitSound.Object);
	
	m_Movement->MaxSpeed = 200.f;
}

bool AMonsterPawn::UseBoidMove()
{
	return GetFocusedTarget() != nullptr;
}
