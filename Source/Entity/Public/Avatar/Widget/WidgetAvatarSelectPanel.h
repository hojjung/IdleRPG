// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetAvatarEle.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "WidgetAvatarSelectPanel.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UWidgetAvatarSelectPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetAvatarEle> m_ClassAvatarEle;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWrapBox* m_WrapParent;
	
	TArray<TWeakObjectPtr<UWidgetAvatarEle>> m_AryEle;
	
protected:
	virtual void NativeOnInitialized() override;

private:
	void OnAvatarLoaded(const FAvatarRow* row);
};
