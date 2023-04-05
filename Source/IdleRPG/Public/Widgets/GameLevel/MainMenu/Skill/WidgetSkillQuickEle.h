// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Delegates/DelegateCombinations.h"
#include "WidgetSkillQuickEle.generated.h"

/**
 * 
 */
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
	
	bool m_bIsRegisterMode;

	int m_nIndex;
	
	
protected:
	virtual void NativeOnInitialized() override;
	
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	void SetIndex(int ndx);
	
	void SetRegisterMode();

	void EquipSkill();
	
	void UnequipSkill();
};
