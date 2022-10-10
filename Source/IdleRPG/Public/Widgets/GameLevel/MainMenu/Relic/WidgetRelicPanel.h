// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetRelicEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Engine/DataTable.h"
#include "Relic/RelicData.h"
#include "Widgets/GameLevel/MainMenu/Gacha/WidgetGachaButton.h"
#include "WidgetRelicPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetRelicPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetRelicEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetGachaButtonPanel* m_GachaBtn;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* m_Scroll;
	UPROPERTY()
	TArray<UWidgetRelicEle*> m_AryEles;

protected:
	virtual void NativeOnInitialized() override;

	void OnUpgradeRelic(const FRelicRow& row);

public:
	UFUNCTION()
	void OnClose();

	void OnOpen();
};
