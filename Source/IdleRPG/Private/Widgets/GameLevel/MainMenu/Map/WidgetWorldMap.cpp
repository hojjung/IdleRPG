// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetWorldMap.h"

#include "BUITween.h"
#include "Components/CanvasPanelSlot.h"

void UWidgetWorldMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_PanelSlot = Cast<UCanvasPanelSlot>(m_CanvasMap->Slot);

}

void UWidgetWorldMap::SetMapCanvasPos(FVector2D ResultPos, bool useAnim)
{
	ResultPos *= -1.f;
	
	FVector2D OutCanvasSize = m_ParentCanvas->GetCachedGeometry().GetLocalSize();

	FVector2D PanelSize = m_PanelSlot->GetSize();

	FVector2D DiffSize = (PanelSize - OutCanvasSize) / 2.0f;  
	
	ResultPos.X = FMath::Clamp<float>(ResultPos.X, -DiffSize.X, DiffSize.X);

	ResultPos.Y = FMath::Clamp<float>(ResultPos.Y, -DiffSize.Y, DiffSize.Y);

	if(useAnim)
	{
		UBUITween::Create(m_PanelSlot.Get()->Content, 0.25f)
			.ToCanvasPosition(ResultPos)
			.Begin();
	}
	else
	{
		m_PanelSlot->SetPosition(ResultPos);	
	}
}

FReply UWidgetWorldMap::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	
	const FVector2D& Delta = InGestureEvent.GetCursorDelta();

	FVector2D Pos = m_PanelSlot->GetPosition();
	
	FVector2D ResultPos = Pos + (Delta * 4.f);

	ResultPos *= -1.f;
	
	SetMapCanvasPos(ResultPos);

	return FReply::Handled();
}


