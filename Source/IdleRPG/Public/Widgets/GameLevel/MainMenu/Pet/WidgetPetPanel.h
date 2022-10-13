// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetPetEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Pet/PetData.h"
#include "Pet/PetManager.h"
#include "Preview/Widget/WidgetPreviewPanel.h"
#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"
#include "WidgetPetPanel.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetPetPanel : public UWidgetMenuBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetPetEle> m_ClassEle;
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
	UWidgetPetEle* m_Equip1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetPetEle* m_Equip2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetPetEle* m_Equip3;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetPetEle* m_Equip4;
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
	TArray<UWidgetPetEle*> m_AryEles;

	TWeakPtr<PetManager> m_PetManager;

	TWeakObjectPtr<UButton> m_CurrentBtn;

	TMap<FName,TArray<UWidgetPetEle*>> m_MapEle;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;

	void CreateAllElements();

	void OnAvatarLoaded(const FName& key, const FPetDataRow& row);

	void OnSelect(const FName& key, const FEntityDataRow& row);
	
	void OnSelectLoaded(FName key, const FPetDataRow* row);

	void SortAvatar();

	void ClearPanel();

	void SetColor(const FColorDataRow& ColorData);

protected:
	void Filter(FName colorID);

	void NoFilter();

protected:
	void Equip(int index, UWidgetPetEle* btn);
	
	void OnClickEquip1(const FName& key, const FEntityDataRow& row);

	void OnClickEquip2(const FName& key, const FEntityDataRow& row);

	void OnClickEquip3(const FName& key, const FEntityDataRow& row);

	void OnClickEquip4(const FName& key, const FEntityDataRow& row);
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
	
public:
	virtual void OnOpen() override;
	
	virtual void OnClose() override;
};
