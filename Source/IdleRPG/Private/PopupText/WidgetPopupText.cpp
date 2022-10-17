// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupText/WidgetPopupText.h"

#include "BUITween.h"
#include "PopupText/PopupTextWComp.h"


UWidgetPopupText::UWidgetPopupText(const FObjectInitializer& objInit):Super(objInit)//m_OnAnimEnd.ExecuteIfBound();
{
}

void UWidgetPopupText::SetTextWant(const FText& textWant, int index)
{
	switch (index)
	{
	case 0:
		m_TextName->SetColorAndOpacity(FLinearColor::White);
		break;
	case 1:
		break;
	case 2:
		break;
	}
	m_TextName->SetText(textWant);

	float RandX = FMath::RandBool() ? FMath::RandRange(100, 200) : FMath::RandRange(-200, -100);

	float RandY = FMath::RandBool() ? FMath::RandRange(-300, -200) : FMath::RandRange(-400, -300);

	SetRenderOpacity(1);
	UBUITween::Create(this,0.7f)
	.FromTranslation(0, 0)
	.ToTranslation(RandX, RandY)
	.FromScale(FVector2D(0.1f))
	.ToScale(FVector2D(1.0f))
	.Easing(EBUIEasingType::OutCubic)
	.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
	{
		UBUITween::Create(Owner, 0.3f)
			.FromOpacity(1)
			.ToOpacity(0)
			.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
			{
				UWidgetPopupText* t = Cast<UWidgetPopupText>(Owner);
				t->m_ParentComponent->EndAnimation();
			}))
			.Begin();
		}))
	.Begin();
}

