// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"
#include "NavigationSystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyLib.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UMyLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
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
