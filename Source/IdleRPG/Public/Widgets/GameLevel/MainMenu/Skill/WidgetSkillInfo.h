// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Skill/SkillData.h"

#include "WidgetSkillInfo.generated.h"

class UWidgetEquipMenuEle;


UCLASS()
class IDLERPG_API UWidgetSkillInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnLevelUp;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnEquip;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipMenuEle* m_ItemEle;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextTier;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextLevel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextCooltime;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	URichTextBlock* m_TextDesc;
	
protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void Close();
	UFUNCTION()
	void LevelUp();
	UFUNCTION()
	void Equip();
	
	void ShowInfo(const FName& id , const FSkillDataRow& row);
};
