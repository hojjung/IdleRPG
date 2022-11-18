#include "Monsters/MonsterPawn.h"
#include "Entity.h"
#include "MyLib.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monsters/Monster.h"
#include "Monsters/Widgets/PawnInfoComp/WidgetPawnInfoComp.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/MyFlockSteering.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& obj): Super(obj.SetDefaultSubobjectClass<UMyFlockSteering>(TEXT("m_Movement")))
{
	m_HitSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio01"));
	m_HitSoundComp->SetupAttachment(RootComponent);
	m_HitSoundComp->SetAutoActivate(false);

	m_CoinSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio02"));
	m_CoinSoundComp->SetupAttachment(RootComponent);
	m_CoinSoundComp->SetAutoActivate(false);

	m_DeathSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio03"));
	m_DeathSoundComp->SetupAttachment(RootComponent);
	m_DeathSoundComp->SetAutoActivate(false);
	

	m_HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleHit02"));
	m_HitParticle->SetupAttachment(RootComponent);

	m_HitParticle->SetRelativeLocation(FVector(0, 0, 55.f));
	m_HitParticle->SetAutoActivate(false);
	m_HitParticle->SetRelativeScale3D(FVector(0.5f));
	m_HitParticle->Deactivate();

	m_CoinParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleHit03"));
	m_CoinParticle->SetupAttachment(RootComponent);

	m_CoinParticle->SetRelativeLocation(FVector(0, 0, 55.f));
	m_CoinParticle->SetAutoActivate(false);
	m_CoinParticle->SetRelativeScale3D(FVector(0.5f));
	m_CoinParticle->Deactivate();
	
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundCoinEffect(
TEXT("ParticleSystem'/Game/03_VisualEffect/PS_CoinDrop.PS_CoinDrop'"));
	
	static ConstructorHelpers::FObjectFinder<USoundBase> FoundHitSound(
		TEXT("SoundWave'/Game/Sound/Use/Fantasy_Game_Weapon_Impact.Fantasy_Game_Weapon_Impact'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> FoundCriHitSound(
		TEXT("SoundWave'/Game/Sound/Use/CriShot-_AudioTrimmer_com___1_.CriShot-_AudioTrimmer_com___1_'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> CoinSound(
		TEXT("SoundWave'/Game/Sound/Use/Coins_01.Coins_01'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> DeathSound(
		TEXT("SoundWave'/Game/Sound/WeaponsNew/Monsters_Sounds_Pro/large_monster_Death/large_monster_Death_1.large_monster_Death_1'"));

	m_HitSoundComp->SetSound(FoundHitSound.Object);
	
	m_CoinSoundComp->SetSound(CoinSound.Object);

	m_HitParticle->SetTemplate(FoundHitEffect.Object);
	
	m_CoinParticle->SetTemplate(FoundCoinEffect.Object);

	m_HitSoundComp->SetSound(FoundHitSound.Object);

	m_DeathSoundComp->SetSound(DeathSound.Object);
	
	m_Movement->SetMaxSpeed(FGlobalVariable::MONSTER_DEFAULT);

	m_fDeathAnimDurationMax = 0;
	
	m_fDeathAnimDurationTimer = 0;

	m_fHitAnimCD = 0;

	SetAtkRange(150);

	m_fAtkDur = 1.0f;
}
void AMonsterPawn::SetEntity(const UUnitAsset* asset)
{
	Super::SetEntity(asset);

	if(m_EntityAsset->m_bAttackFirst)
	{
		AMyPlayerPawn* Play = UMyGameInstance::Get->GetPlayerPawn();
		
		SetFocusedTarget(Play);
	}

	float Z =  m_BodyMesh->Bounds.BoxExtent.Z;

	m_PawnInfo->SetRelativeLocation(FVector(0,0,Z));

	m_HitParticle->SetRelativeLocation(FVector(0,0,Z * 0.3f));

	m_CoinParticle->SetRelativeLocation(FVector(0,0,Z * 0.75f));

	m_SpawnPos = GetActorLocation();
}

void AMonsterPawn::Revive()
{
	Super::Revive();
	//
	FName MaskParam = TEXT("Visibility");

	m_BodyMesh->SetScalarParameterValueOnMaterials(MaskParam, 1.0f);

	FName DurationParamName = TEXT("Duration");

	m_BodyMesh->SetScalarParameterValueOnMaterials(DurationParamName, 0.25f);

	FName TimeParamName = TEXT("StartTime");

	float TimeSec = 0;

	m_BodyMesh->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);


}

void AMonsterPawn::OnReviveAnimEnd()
{
	if(m_EntityAsset->m_bAttackFirst)
	{
		AMyPlayerPawn* Play = UMyGameInstance::Get->GetPlayerPawn();
		
		SetFocusedTarget(Play);
	}
	
	Super::OnReviveAnimEnd();
}

void AMonsterPawn::SetGas(TSharedPtr<GAS> newGas)
{
	Super::SetGas(newGas);

	m_Gas.Pin()->m_OnHpChanged.AddUObject(this, &AMonsterPawn::OnHpChanged);

	m_Gas.Pin()->m_OnTookDamage.AddUObject(this, &AMonsterPawn::OnTookDamage);
}
void AMonsterPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsAlive())
	{
		SetDeathEffectMaterial(DeltaSeconds);
		return;
	}

	m_fHitAnimCD -= DeltaSeconds;
	
	m_fAttackCD -= DeltaSeconds;
}

bool AMonsterPawn::UseBoidMove()
{
	return GetFocusedTarget() != nullptr;
}

void AMonsterPawn::PlayTookHitMontage()
{
	if(m_EntityAsset->m_TookHitMontage && m_fHitAnimCD < 0.f)
	{
		PlayAnimMontage(m_EntityAsset->m_TookHitMontage.Get(),1);

		m_fHitAnimCD = FMath::RandRange(1.5f,2.5f);
	}
}

void AMonsterPawn::PlayHitFlash()
{
	FName TimeParamName = "StartTime";
	
	float TimeSec = UGameplayStatics::GetTimeSeconds(GetWorld());

	m_BodyMesh->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

}

void AMonsterPawn::PlayHitEffect()
{
	m_HitParticle->Activate(true);
}

void AMonsterPawn::PlayCoinEffect()
{
	m_CoinParticle->Activate();
	
	m_CoinSoundComp->Play();
}

void AMonsterPawn::PlayHittenSound(EDamagePopup pop)
{
	m_HitSoundComp->Play();
}

void AMonsterPawn::OnHpChanged()
{
	if(!m_PawnInfo->IsVisible())
	{
		m_PawnInfo->SetVisibility(true);
	}
	m_PawnInfo->SetPawnInfo(this);
}

void AMonsterPawn::CreateSetDeathCurve(float fullLength)
{
	m_CurveDeathAnim =FFloatCurve(); 
	m_CurveDeathAnim.UpdateOrAddKey(1, 0);
	m_CurveDeathAnim.UpdateOrAddKey(0, fullLength);
}

void AMonsterPawn::SetDeathEffectMaterial(float deltaTime)
{
	m_fDeathAnimDurationTimer+=deltaTime;

	float Value = m_CurveDeathAnim.Evaluate(m_fDeathAnimDurationTimer);

	FName MaskParam = TEXT("Visibility");

	m_BodyMesh->SetScalarParameterValueOnMaterials(MaskParam, Value);
}

void AMonsterPawn::StartDeathEffectMaterial(float duration)
{
	m_fDeathAnimDurationMax = duration;

	m_fDeathAnimDurationTimer = 0.f;

	CreateSetDeathCurve(duration);

	FName TimeParamName = TEXT("StartTime");

	FName DurationParamName = TEXT("Duration");

	float TimeSec = UGameplayStatics::GetTimeSeconds(GetWorld());

	m_BodyMesh->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

	m_BodyMesh->SetScalarParameterValueOnMaterials(DurationParamName, duration);
}

void AMonsterPawn::OnTookDamage(ACombatPawn* other, BigInt dmg, EDamagePopup pop)
{
	SetFocusedTarget(other);
	PlayHittenSound(pop);
	PlayHitFlash();
	PlayTookHitMontage();
	PlayHitEffect();
	UMyGameInstance::Get->GetPlayerCon()->ShowInGameWorldText(dmg, this, pop);
	//m_PlCon->ShowDamageNumber(amount,this,pp);
}

void AMonsterPawn::OnDead()
{
	Super::OnDead();
	m_PawnInfo->SetVisibility(false);
	PlayCoinEffect();
	PlayDeathSound();

	UMyGameInstance::Get->GetStageMode()->OnMonsterDead(this);
}

void AMonsterPawn::PlayDeathSound()
{
	m_DeathSoundComp->Play();
}

void AMonsterPawn::PlayDeathAnim()
{
	if(m_EntityAsset->m_DeathMontage)
	{
		PlayAnimMontage(m_EntityAsset->m_DeathMontage.Get());

		float AnimLength = m_EntityAsset->m_DeathMontage->GetPlayLength() - 0.4f;

		GetWorldTimerManager().SetTimer(m_DeathAnimTimer, this, &AMonsterPawn::OnDeathAnimEnd, AnimLength, false);

		StartDeathEffectMaterial(m_EntityAsset->m_DeathMontage->GetPlayLength());
	}
	else
	{
		OnDeathAnimEnd();
	}
}

void AMonsterPawn::OnDeathAnimEnd()
{
	Super::OnDeathAnimEnd();
	//Respawn?
	UMyGameInstance::Get->GetStageMode()->OnMonsterDeadAnimEnd(this);
}
