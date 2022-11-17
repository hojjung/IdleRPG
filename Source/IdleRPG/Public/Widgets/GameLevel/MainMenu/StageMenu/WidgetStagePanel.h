// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetStageEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Manager/MyGameInstance.h"
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
	
	int m_nMaxMap;

	int m_nMapIndex;

protected:
	virtual void NativeOnInitialized() override;

	void OnMapChanged();

	void UpdateStage();

	void Clear();
	
public:
	UFUNCTION()
	void OnClickLeft();
	UFUNCTION()
	void OnClickRight();
};
