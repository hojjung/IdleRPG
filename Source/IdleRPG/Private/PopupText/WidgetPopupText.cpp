// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupText/WidgetPopupText.h"

#include "BUITween.h"
#include "PopupText/PopupTextWComp.h"


UWidgetPopupText::UWidgetPopupText(const FObjectInitializer& objInit):Super(objInit)//m_OnAnimEnd.ExecuteIfBound();
{
}

void UWidgetPopupText::SetTextWant(const FText& textWant, int index)
{
	m_TextName->SetText(textWant);

	float RandX = FMath::RandRange(-100, 100);

	float RandY = FMath::RandRange(0, 100);

	UBUITween::Create(this,0.5f)
	.ToTranslation(300, 300)
	.Easing(EBUIEasingType::OutCubic)
	.OnComplete( FBUITweenSignature::CreateLambda([]( UWidget* Owner )
		{
		UWidgetPopupText* t = Cast<UWidgetPopupText>(Owner); 
			t->m_ParentComponent->EndAnimation();
		}))
	.Begin();
}

