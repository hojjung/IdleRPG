// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/HUD/WidgetAlertPanel.h"

#include "Components/VerticalBoxSlot.h"


void UWidgetAlertPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(m_ClassInfo);
	
	m_AryInfo.Reset();

	int	iter = 0;

	while (iter < 10)
	{
		UWidgetAlertText* ItemEle = CreateWidget<UWidgetAlertText>(this, m_ClassInfo);

		ItemEle->SetVisibility(ESlateVisibility::Collapsed);
		
		m_AryInfo.Add(ItemEle);
		
		UVerticalBoxSlot* ChildSlot = Cast<UVerticalBoxSlot>(m_OverlayInfo->AddChild(ItemEle));
		
		ChildSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		
		ChildSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
		
		ChildSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		
		ItemEle->SetRenderScale(FVector2D(1.f,-1.f));
		
		iter++;
	}
}

UWidgetAlertText* UWidgetAlertPanel::GetTextElement()
{
	UWidgetAlertText* TextEle = m_AryInfo[m_nIndex];

	m_nIndex++;

	if (m_nIndex >= m_AryInfo.Num())
	{
		m_nIndex = 0;
	}
	return TextEle;
}

void UWidgetAlertPanel::ShowText(const FText& str, const FLinearColor& color)
{
	GetTextElement()->Show(str, color);
}
