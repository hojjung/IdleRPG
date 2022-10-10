// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetEquipInfo.h"
#include "WidgetEquipMenuEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/WrapBox.h"
#include "Widgets/GameLevel/MainMenu/Gacha/WidgetGachaButton.h"
#include "WidgetEquipMenu.generated.h"

/**
 * 무기,갑옷 모든것의 패널
 * 
 */
UCLASS()
class IDLERPG_API UWidgetEquipMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (ExposeOnSpawn))
	UTexture2D* m_GachaIcon;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetEquipMenuEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetGachaButtonPanel* m_GachaBtn;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UUniformGridPanel* m_Grid;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEquipInfo* m_EquipInfo;
	UPROPERTY()
	TArray<UWidgetEquipMenuEle*> m_AryEles;
	
public:
	void Init(FText panelName, const UDataTable* equipItems, int costPerOne);

	void UpdatePanel();
	
	void OnShow();

	void OnClose();

protected:
	void Sort();
	
	void OnSelectEquip(const FEquipRow& row);

	virtual void NativePreConstruct() override;
};
