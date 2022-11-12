// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetWorldMap.h"

#include "BUITween.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/GameLevel/MainMenu/Map/WidgetMenuMap.h"

void UWidgetWorldMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_PanelSlot = Cast<UCanvasPanelSlot>(m_CanvasMap->Slot);

	m_StoryElements.Reset(10);

	m_ContentElement.Reserve(10);

	TArray<UWidget*> Childs = m_CanvasMap->GetAllChildren();
	
	for(UWidget* ChildEle : Childs)
	{
		UWidgetMapEle* Ele = Cast<UWidgetMapEle>(ChildEle);

		if(!Ele)
		{
			continue;
		}
		
		
		const FString& Zone = Ele->GetZoneID().ToString();

		if(Zone.Contains(TEXT("Story")))
		{
			m_StoryElements.Add(Ele);
			continue;
		}
		m_ContentElement.Add(Ele->GetZoneID(),Ele);
	}
	
	m_StoryElements.Sort([](const TWeakObjectPtr<UWidgetMapEle>& LHS, const TWeakObjectPtr<UWidgetMapEle>& RHS)
	{
		FName LhsLevel = LHS->GetZoneID();

		FName RhsLevel = RHS->GetZoneID();

		return LhsLevel.Compare(RhsLevel) < 0;
	});
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

void UWidgetWorldMap::MoveToContent(FName id)
{
	TWeakObjectPtr<UWidgetMapEle>* EleFound = m_ContentElement.Find(id);
	
	if(!EleFound)
	{
		return;
	}
	
	TWeakObjectPtr<UWidgetMapEle> Ele = *EleFound;

	SetMapCanvasPos(Ele->GetPos(), true);
}

void UWidgetWorldMap::MoveToStoryUnlock(int index)
{
	TWeakObjectPtr<UWidgetMapEle> Ele = m_StoryElements[index];

	SetMapCanvasPos(Ele->GetPos(), true);
}

void UWidgetWorldMap::SetOnClickEvent(UWidgetMapEle::FOnClick onClick)
{
	for(auto Ele : m_StoryElements)
	{
		Ele->m_OnClick = onClick;
	}
	for(auto Ele : m_ContentElement)
	{
		Ele.Value->m_OnClick = onClick;
	}
}
