// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayFabClientDataModels.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "GameLevel/HUD/WidgetAlertPanel.h"
#include "WidgetLoginCanvas.generated.h"



class UWidgetContract;
class UWidgetConfirmPanel;
class FWebServiceModule;
UCLASS()
class IDLERPG_API UWidgetLoginCanvas : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgBlink;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetAlertPanel* m_AlertInfoWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetContract* m_WidgetContract;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_NewsCanvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTitleNews;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCloseNews;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetConfirmPanel* m_ConfirmPanel;
	
	float m_fTimer;

	const FWebServiceModule* m_WebService;
public:
	void PrintInfoText(const FString& str, FLinearColor color);
	
	UWidgetConfirmPanel* GetConfirmPanel();

protected:
	virtual void NativeOnInitialized() override;
	
	void OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult& rslt);

	void OpenConfirmPanel();
	FReply TryStart();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UFUNCTION()
	void OnCloseNews();

	void StartPlayfabLogin();
};
