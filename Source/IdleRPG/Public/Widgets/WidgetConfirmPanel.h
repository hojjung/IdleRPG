// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayFabClientDataModels.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "WidgetConfirmPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetConfirmPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE(FOnClick);

	FOnClick m_OnCancel;

	FOnClick m_OnConfirm;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	void SetConfirmPanel(const FString&& str,FOnClick onCancel, FOnClick onConfirm);
	
	void SetConfirmPanel(const FString& str,FOnClick onCancel, FOnClick onConfirm);

	void SetConfirmPanel(const FText& txt,FOnClick onCancel, FOnClick onConfirm);
	
	void SetConfirmPanel(const FText&& txt,FOnClick onCancel, FOnClick onConfirm);

public:
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void OnConfirm();
};
