// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetStageEle.h"
#include "WidgetMapSelect.generated.h"

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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetStageEle> m_ClassEle;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClose();
	
	void Open(const FName& id);
};
