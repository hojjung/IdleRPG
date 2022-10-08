// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetAvatarEle.h"
#include "Entity.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Player/Avatar/AvatarManager.h"
#include "Preview/Widget/WidgetPreviewPanel.h"
#include "WidgetAvatarPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetAvatarPanel : public UUserWidget
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
	UPROPERTY()
	TArray<UWidgetAvatarEle*> m_AryEles;

	int m_TotalCount;

	TWeakPtr<AvatarManager> m_AvatarManager;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	void CreateAllElements();

	void OnAvatarLoaded(const FAvatarRow* row);

	void OnSelect(const FAvatarRow& row);
	
	void OnSelectLoaded(const FAvatarRow* row);

	void SortAvatar();
	
public:
	void OnOpen();
	UFUNCTION()
	void OnClose();
};
