// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetMapSelect.h"
#include "WidgetWorldMap.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"
#include "WidgetMenuMap.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMenuMap : public UWidgetMenuBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetWorldMap* m_Map;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMapSelect* m_Select;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnStory;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnMine;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnPVP;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnElf;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnJapan;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnBoneDragon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnReaper;

	TWeakObjectPtr<UButton> m_Current;
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClickStory();
	UFUNCTION()
	void OnClickMine();
	UFUNCTION()
	void OnClickJapan();
	UFUNCTION()
	void OnClickColosseum();
	UFUNCTION()
	void OnClickElf();
	UFUNCTION()
	void OnClickRaid01();
	UFUNCTION()
	void OnClickRaid02();

	void OnClickBtn(const FName& mapZoneID);
};
