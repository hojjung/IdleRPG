// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Delegates/DelegateCombinations.h"
#include "WidgetSkillQuickEle.generated.h"

/**
 * 
 */

struct FSkillInven;

UCLASS()
class IDLERPG_API UWidgetSkillQuickEle : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClick,int);

	FOnClick m_OnClick;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgSkillIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgSkillAdd;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UOverlay* m_OverlayCD;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgCD;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextCd;
	
	bool m_bIsRegisterMode;

	int m_nIndex;
	
	const FSkillInven* m_SkillInst;

	float m_fMaxCooldown;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateCoolTime();

	void UseSkill();

public:
	void SetIndex(int ndx);
	
	void SetRegisterMode();

	void EquipSkill();
	
	void UnequipSkill();

	void Update(const FSkillInven* skillInst);
};
