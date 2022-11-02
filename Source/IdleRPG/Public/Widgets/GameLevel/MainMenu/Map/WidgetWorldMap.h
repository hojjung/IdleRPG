// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetMapEle.h"
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

	TArray<TWeakObjectPtr<UWidgetMapEle>> m_StoryElements;

	TMap<FName, TWeakObjectPtr<UWidgetMapEle>> m_ContentElement;

protected:
	
	
protected:
	virtual void NativeOnInitialized() override;
	
	void SetMapCanvasPos(FVector2D ResultPos, bool useAnim = false);

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	void MoveToContent(FName id);

	void MoveToStoryUnlock(int index);
public:
	UFUNCTION()
	void OnClickStory();
	UFUNCTION()
	void OnClickJapan();
	UFUNCTION()
	void OnClickColosseum();
};
