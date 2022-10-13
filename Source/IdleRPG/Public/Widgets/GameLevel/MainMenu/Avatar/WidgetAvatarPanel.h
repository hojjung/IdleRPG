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
#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"
#include "WidgetAvatarPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetAvatarPanel : public UWidgetMenuBase
{
	GENERATED_BODY()

protected:
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
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetAvatarEle* m_Equip;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetAvatarEle* m_Skin;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnAll;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnTier1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnTier2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnTier3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnTier4;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnTier5;
	UPROPERTY()
	TArray<UWidgetAvatarEle*> m_AryEles;

	TWeakPtr<AvatarManager> m_AvatarManager;
	
	TWeakObjectPtr<UButton> m_CurrentBtn;

	TMap<FName,TArray<UWidgetAvatarEle*>> m_MapEle;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	void CreateAllElements();

	void OnAvatarLoaded(const FName& key, const FAvatarRow& row);

	void OnSelect(const FName& key, const FEntityDataRow& row);
	
	void OnSelectLoaded(FName key, const FAvatarRow* row);

	void SortAvatar();

	void ClearPanel();

	void OnClickEquip(const FName& key, const FEntityDataRow& row);

	void OnClickSkin(const FName& key, const FEntityDataRow& row);

	void SetColor(const FColorDataRow& ColorData);

	void Filter(FName colorID);

	void NoFilter();
	
public:
	virtual void OnOpen() override;
	
	virtual void OnClose() override;

public:
	UFUNCTION()
	void OnClickAll();
	UFUNCTION()
	void OnClickTier1();
	UFUNCTION()
	void OnClickTier2();
	UFUNCTION()
	void OnClickTier3();
	UFUNCTION()
	void OnClickTier4();
	UFUNCTION()
	void OnClickTier5();
};

