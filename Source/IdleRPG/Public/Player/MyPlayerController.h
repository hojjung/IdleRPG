// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

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

protected:
	TSubclassOf<UWidgetMainCanvas> m_ClassCanvas;
	UPROPERTY()
	UWidgetMainCanvas* m_Canvas;
	//UPROPERTY(VisibleAnywhere)
	//TArray<UInGameTextWidgetComp*> m_AryIGWC;
	UPROPERTY()
	TArray<AActor*> m_AryIgnoreActors;
	UPROPERTY()
	int m_CompUseIndex;
	UPROPERTY()
	UTouchInterface* m_Joystick;

	FVector2D m_MousePos;

	bool m_bUseFlick;
	
protected:
	virtual void BeginPlay() override;

	//void CreateIGWC(int count);

	//UInGameTextWidgetComp* GetIGWC(); 

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	void OpenExitPanel();

	void ExitGame();

	void OnPressed();

	void OnReleased();

public:
	//void ShowInGameWorldText(float number,ABaseUnitPawn* interactActor,ETextType dmgPopup); //target
	
	//void ShowInGameWorldText(const FString& stringWant, const ABaseUnitPawn* interactActor, ETextType dmgPopup); //target

	UFUNCTION()
	void BackToSelectMenu();
	UFUNCTION()
	void OnTouchPressed();

	void EnableJoystick(bool b);
};
