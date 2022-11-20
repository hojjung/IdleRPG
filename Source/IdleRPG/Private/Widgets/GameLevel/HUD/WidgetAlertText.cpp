// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/HUD/WidgetAlertText.h"

#include "BUITween.h"
#include "BUITweenInstance.h"

void UWidgetAlertText::Show(const FText& str, FLinearColor color)
{
	m_TextName->SetText(str);
	m_TextName->SetColorAndOpacity(color);
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	FBUITweenInstance& Inst = UBUITween::Create(this, 1);

	Inst.FromOpacity(1).
	ToOpacity(0).
	OnComplete( FBUITweenSignature::CreateLambda([&]( UWidget* Owner )
		{
			Owner->SetVisibility(ESlateVisibility::Collapsed);
			
		}))
	.Begin();
}
