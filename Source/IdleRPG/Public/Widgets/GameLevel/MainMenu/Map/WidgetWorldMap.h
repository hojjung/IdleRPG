// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "WidgetWorldMap.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetWorldMap : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_ParentCanvas;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasMap;
	TWeakObjectPtr<UCanvasPanelSlot> m_PanelSlot;

protected:
	virtual void NativeOnInitialized() override;
	
	void SetMapCanvasPos(FVector2D ResultPos, bool useAnim = false);

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
};
