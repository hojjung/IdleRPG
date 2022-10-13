// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"
#include "WidgetStageEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"
#include "WidgetStagePanel.generated.h"

struct FStageRow;
/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetStagePanel : public UWidgetMenuBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetStageEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnLeft;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnRight;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* m_Scroll;
	TArray<const FStageRow*> m_AryRows;
	TArray<UWidgetStageEle*> m_AryEles;
	
	int m_nMaxStage;

	int m_nStageLevel;
protected:
	virtual void NativeOnInitialized() override;

	void OnMapChanged(EGameMode m , int level);

	void UpdateStage();

	void Clear();
	
public:
	UFUNCTION()
	void OnClickLeft();
	UFUNCTION()
	void OnClickRight();
};
