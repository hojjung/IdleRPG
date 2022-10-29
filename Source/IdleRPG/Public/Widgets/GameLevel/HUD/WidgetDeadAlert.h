// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetDeadAlert.generated.h"

class UWidgetMainCanvas;
/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetDeadAlert : public UUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<UWidgetMainCanvas> m_ParentCanvas;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnSkill;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnAcce;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnRelic;
	
public:
	void Init(UWidgetMainCanvas* parent);

public:
	UFUNCTION()
	void OnOpenUpgrade();
	UFUNCTION()
	void OnOpenSkill();
	UFUNCTION()
	void OnOpenAcce();
	UFUNCTION()
	void OnOpenRelic();

protected:
	virtual void NativeOnInitialized() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
