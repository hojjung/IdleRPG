// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetSkillQuickEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "WidgetSkillQuickPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetSkillQuickPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int m_nMaxSkillCount = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int m_nInitSkillCount = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetSkillQuickEle> m_ClassSkillQuick;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* m_ScrollSkills;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnAuto;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgTrail; 
	
	
protected:
	virtual void NativeOnInitialized() override;
};
