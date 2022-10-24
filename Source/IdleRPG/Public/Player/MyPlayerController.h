// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatPawn.h"
#include "GameFramework/PlayerController.h"
#include "GAS/BigInt/BigIntLib.h"
#include "PopupText/PopupTextPool.h"
#include "MyPlayerController.generated.h"

class SSlateEleBase;
class UWidgetMainCanvas;
/**
 * 
 */
UCLASS()
class IDLERPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTouchWorld,const FHitResult&);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFlick,const FVector2D&);

	FOnTouchWorld m_OnTouch;

	FOnFlick m_OnFlick;
	
public:
	AMyPlayerController();

	TSharedPtr<SSlateEleBase> m_Ele;
protected:
	UPROPERTY()
	TArray<AActor*> m_AryIgnoreActors;
	UPROPERTY()
	int m_CompUseIndex;
	UPROPERTY()
	UTouchInterface* m_Joystick;

	FVector2D m_MousePos;

	TSharedPtr<PopupTextPool> m_PopupTextPool;

	bool m_bUseFlick;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	void OpenExitPanel();

	void ExitGame();

	void OnPressed();

	void OnReleased();

public:
	void ShowInGameWorldText(BigInt number, const ACombatPawn* interactActor, EDamagePopup dmgPopup); //target
	
	void ShowInGameWorldText(const FString& stringWant, const ACombatPawn* interactActor, EDamagePopup dmgPopup); //target

	UFUNCTION()
	void BackToSelectMenu();
	UFUNCTION()
	void OnTouchPressed();

	void EnableJoystick(bool b);
};
