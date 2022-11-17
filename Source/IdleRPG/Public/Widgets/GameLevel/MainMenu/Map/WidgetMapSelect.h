// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "WidgetMapSelect.generated.h"

class UWidgetMapSelectEle;
class UWidgetStageEle;
struct FContentDataRow;
class UWidgetMapInfo;
/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetMapSelect : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* m_Scroll;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetMapInfo* m_MapInfo;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetMapSelectEle> m_ClassEle;

	const FContentDataRow* m_ContentData;
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClose();
	
	void Open(const FName& id);

	void SetInfoPanel(int lv);
};
