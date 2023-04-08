// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/MyGameInstance.h"
#include "Manager/StageMode/StageModeBase.h"
#include "MyLib.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UMyLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <class T>
	static void GetNearActors(const AActor* caller, TArray<T*>& outAry, float range, bool traceOut = true)
	{
		UMyGameInstance::Get->m_StageMode->GetNearNpcs(caller, outAry, range, traceOut);
	}
	template <class T>
	static void GetNearActors(const FVector& center, TArray<T*>& outAry, float range)
	{
		UMyGameInstance::Get->m_StageMode->GetNearNpcs(center, outAry, range);
	}
	static bool CheckAngle(const AActor* center, const AActor* target, float angle)
	{
		float PeripheralVisionCosine = FMath::Cos(FMath::DegreesToRadians(angle));

		FVector const OtherLoc = target->GetActorLocation();

		FVector const SensorLoc = center->GetActorLocation();

		FVector const SelfToOther = OtherLoc - SensorLoc;

		FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();

		FVector const MyFacingDir = center->GetActorRotation().Vector();

		return ((SelfToOtherDir | MyFacingDir) >= PeripheralVisionCosine);
	}

	static bool CheckAngle(const FVector centerDir,const FVector centerPos, const AActor* target, float angle)
	{
		FVector const OtherLoc = target->GetActorLocation();
	
		const FVector& ActorDirection = (OtherLoc - centerPos).GetSafeNormal();
	
		const float DotProduct = FVector::DotProduct(ActorDirection, centerDir);

		const float LowerLimit =  FMath::Cos(FMath::DegreesToRadians(angle / 2));
	
		constexpr float UpperLimit = 1.0f;
			
		return DotProduct >= LowerLimit && DotProduct <= UpperLimit;
	}

	static UNavigationSystemV1* GetNavSys()
	{
		return FNavigationSystem::GetCurrent<UNavigationSystemV1>(UMyGameInstance::Get);	
	}
	
	static void SnapActorToNav(AActor* want)
	{
		FVector ActorLoc = want->GetActorLocation();
		FNavLocation Loc;
		if(!GetNavSys()->ProjectPointToNavigation(ActorLoc,Loc))
		{
			GetNavSys()->GetRandomPointInNavigableRadius(ActorLoc,1000,Loc);
		}
		FVector Extent;
	
		want->GetActorBounds(true, ActorLoc, Extent);
	
		FVector NewLoc = Loc;
	
		NewLoc.Z += Extent.Z;
	
		want->SetActorLocation(NewLoc);
	}
};
