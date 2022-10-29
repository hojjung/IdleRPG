#include "Animations/AnimNotifySt_AOECircle.h"
#include "MyLib.h"
#include "Components/DecalComponent.h"


UAnimNotifySt_AOECircle::UAnimNotifySt_AOECircle(const FObjectInitializer& obj): Super(obj)
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMat(TEXT("Material'/Game/03_VisualEffect/Decals/MAT_AOE_Circle.MAT_AOE_Circle'"));
	
	m_MatDecalTemplate = FoundMat.Object;

	m_DecalColorWant = FLinearColor::Red;

	m_fTimer = 0;

	m_fDurationMax = 0;

	m_fDamage = 30.f;

	m_RotYaw = 90.f;
}

void UAnimNotifySt_AOECircle::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACombatPawn* CPawn =  MeshComp->GetOwner<ACombatPawn>();
	if(CPawn)
	{
		CPawn->SetRotateAble(false);
		
		m_StartPos = CPawn->GetActorLocation();

		m_StartDir = CPawn->GetActorForwardVector();
	}
	m_MatDynamic = UMaterialInstanceDynamic::Create(m_MatDecalTemplate, this);

	if(CPawn)
	{
		UDecalComponent* ReuseComp = Cast<UDecalComponent>(CPawn->FindComp(this));
		if(!ReuseComp)
		{
			m_Decal = NewObject<UDecalComponent>(MeshComp);
			CPawn->AddComp(this, m_Decal.Get());
		}
		else
		{
			m_Decal = ReuseComp;
			m_Decal->SetVisibility(true);
		}
	}
	else
	{
		m_Decal = NewObject<UDecalComponent>(MeshComp);
	}
	m_Decal->SetDecalMaterial(m_MatDynamic);
	if(!m_Decal->IsRegistered())
	{
		m_Decal->RegisterComponentWithWorld(MeshComp->GetWorld());
	}
	m_Decal->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform);
	m_Decal->SetRelativeRotation(FRotator(-90.f,m_RotYaw,0.f));
	m_Decal->DecalSize = FVector(256.f, m_Radius, m_Radius);

	float RadDegree = FMath::Clamp(m_EulerAngle / 360.f, 0.f, 1.f);
	m_MatDynamic->SetScalarParameterValue(TEXT("RadianAngle"), RadDegree);
	m_MatDynamic->SetVectorParameterValue(TEXT("Color"), m_DecalColorWant);
	m_MatDynamic->SetScalarParameterValue(TEXT("Value"), 0.f);

	m_fDurationMax = TotalDuration / Animation->RateScale;
	
	m_fTimer = 0;
	//
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UAnimNotifySt_AOECircle::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
#if UE_EDITOR
	if(!m_MatDynamic)
	{
		Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
		return;
	}
#endif
	m_fTimer += FrameDeltaTime;

	float Percent = 0.f;
	
	if(m_fTimer > 0.f)
	{
		Percent = m_fTimer / m_fDurationMax;
	}
	m_MatDynamic->SetScalarParameterValue(TEXT("Value"), Percent);
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

bool UAnimNotifySt_AOECircle::TraceDamage(ACombatPawn* CPawn)
{
	TArray<ACombatPawn*> Hits;

	if (!TraceSphere(CPawn, Hits, m_Radius))
	{
		return false;
	}
	bool HitCone = false;
	
	for (AActor* Mob : Hits)
	{
		if (!UMyLib::CheckAngle(m_StartDir, m_StartPos, Mob, m_EulerAngle))
		{
			continue;
		}

		HitCone = true;

		ACombatPawn* CombatPawn = Cast<ACombatPawn>(Mob);

		CombatPawn->MyTakeDamage(CPawn, m_DmgType);
	}
	return HitCone;
}

void UAnimNotifySt_AOECircle::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(m_Decal.IsValid())
	{
		m_Decal->SetVisibility(false);
		m_Decal = nullptr;
	}
	
	ACombatPawn* CPawn =  MeshComp->GetOwner<ACombatPawn>();
	
	FAnimMontageInstance* CurrentMont = MeshComp->GetAnimInstance()->GetActiveMontageInstance();
	
	if(CPawn &&  CurrentMont && CurrentMont->Montage == Animation)
	{
		CPawn->SetRotateAble(true);
		
		bool Hit = TraceDamage(CPawn);

		if(Hit && m_ClassCamShake->IsValidLowLevel())
		{
			UMyGameInstance::Get->GetPlayerCon()->ClientStartCameraShake(m_ClassCamShake);
		}
	}
	
	m_fTimer = 0;
	
	m_fDurationMax = 0.f;

	m_MatDynamic = nullptr;

	Super::NotifyEnd(MeshComp, Animation);
}

bool UAnimNotifySt_AOECircle::TraceSphere(const ACombatPawn* instigator, TArray<ACombatPawn*>& outHits, float radius)
{
	UMyGameInstance::Get->GetNearNpcs(instigator, outHits, radius);
	
	return outHits.Num() > 0;
}