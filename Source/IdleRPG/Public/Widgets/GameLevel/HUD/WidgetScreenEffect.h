// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUITweenInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "IdleRPG.h"
#include "WidgetScreenEffect.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetScreenEffect : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgBlack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgTakeHit;
	
protected:
	float m_fFadeOutTimer;

	float m_fMaxFadeOut;

	FVoidvoid m_OnFadeOutDone;

	bool IsFadeOut;

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void EndFade();

public:
	void ShowFadeOut(float t, const FVoidvoid& onFadeOutEnd);

	void ShowHittenFlash();

	void HideFadeOut();
};

UCLASS()
class IDLERPG_API UWidgetAnimLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void PlayTextAnim(UTextBlock* textblock, FLinearColor colorWant, float dur);
	
	static FBUITweenInstance& PlayAlphaFlashAnim(UImage* textblock, float dur);

protected:
	static void RecoverTextAnim(UTextBlock* textblock, float dur);

	static void RecoverAlphaFlashAnim(UImage* textblock, float dur);
};
