// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetAvatarEle.h"
#include "Avatar/AvatarManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Preview/Widget/WidgetPreviewPanel.h"
#include "WidgetAvatarPanel.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UWidgetAvatarPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetAvatarEle> m_ClassEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWrapBox* m_Wrap;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* m_ImgGlow;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* m_TextTierName;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetPreviewPanel* m_Preview;

	TSharedPtr<AvatarManager> m_AvatarManager;
	
protected:
	virtual void NativeOnInitialized() override;

	void CreateAllElements();

	void OnSelect(const FAvatarRow* row);
	
	void OnSelectLoaded(const FAvatarRow* row);
	
public:
	void OnOpen();
	UFUNCTION()
	void OnClose();
};
