// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetSkillEle.h"
#include "WidgetSkillInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"
#include "Widgets/GameLevel/MainMenu/Gacha/WidgetGachaButton.h"
#include "WidgetSkillPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetSkillPanel : public UWidgetMenuBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (ExposeOnSpawn))
	UTexture2D* m_GachaIcon;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetSkillEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetGachaButtonPanel* m_GachaBtn;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UUniformGridPanel* m_Grid;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetSkillInfo* m_SkillInfo;
	UPROPERTY()
	TArray<UWidgetSkillEle*> m_AryEles;
	
public:
	void OnShow();

	virtual void OnClose() override;
	
protected:
	void Init(FText panelName, int costPerOne);

	void UpdatePanel();

	virtual void NativeOnInitialized() override;
	
	void Sort();
	
	void OnSelectSkill(const FName& id , const FEntityDataRow& row);

	virtual void NativePreConstruct() override;
};
