// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Preview/WidgetPreviewPanel.h"


void UWidgetPreviewPanel::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	m_PreviewImage->SetBrushFromMaterial(m_PreviewMat);
}

void UWidgetPreviewPanel::NativeDestruct()
{
	Super::NativeDestruct();
	m_PreviewProxy = nullptr;
}

void UWidgetPreviewPanel::Init(UObject* proxyObj)
{
	m_PreviewProxy.SetObject(proxyObj);
	m_PreviewProxy.SetInterface(Cast<IPreviewProxy>(proxyObj));
}

FReply UWidgetPreviewPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	m_PreviewProxy->SetIsTouched(true);
	
	return FReply::Handled();
}

FReply UWidgetPreviewPanel::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	m_PreviewProxy->SetIsTouched(false);

	return FReply::Handled();
}

void UWidgetPreviewPanel::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	m_PreviewProxy->SetIsTouched(false);
}

FReply UWidgetPreviewPanel::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	
	float DeltaX = InGestureEvent.GetCursorDelta().X;

	if(DeltaX != 0.f)
	{
		m_PreviewProxy->RotatePawn(DeltaX);
	}

	return FReply::Handled();
}


