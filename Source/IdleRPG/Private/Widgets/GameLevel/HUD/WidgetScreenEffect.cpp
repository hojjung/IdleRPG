// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/HUD/WidgetScreenEffect.h"

#include "BUITween.h"


void UWidgetScreenEffect::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_ImgTakeHit->SetRenderOpacity(0.f);
	m_ImgTakeHit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	EndFade();
}

void UWidgetScreenEffect::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_fMaxFadeOut <= -1)
	{
		return;
	}

	if(IsFadeOut)
	{
		m_fFadeOutTimer += InDeltaTime;
	}
	else
	{
		m_fFadeOutTimer -= InDeltaTime;
	}

	float Per = m_fFadeOutTimer / m_fMaxFadeOut;
	
	m_ImgBlack->SetRenderOpacity(Per);

	if(m_fFadeOutTimer >= m_fMaxFadeOut)
	{
		m_OnFadeOutDone.ExecuteIfBound();
		//EndFade();
	}
}

void UWidgetScreenEffect::EndFade()
{
	m_ImgBlack->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ImgBlack->SetRenderOpacity(0);
	
	m_fMaxFadeOut = -1;

	m_fFadeOutTimer = -1;
	
	IsFadeOut = false;
	
	m_OnFadeOutDone.Unbind();
}

void UWidgetScreenEffect::ShowFadeOut(float t, const FVoidvoid& onFadeOutEnd)
{
	if(IsFadeOut)
	{
		return;
	}

	IsFadeOut = true;
	
	m_ImgBlack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_fFadeOutTimer = 0;
	
	m_fMaxFadeOut = t;

	m_OnFadeOutDone = onFadeOutEnd;
}

void UWidgetScreenEffect::ShowHittenFlash()
{
	if(UBUITween::GetIsTweening(m_ImgTakeHit))
	{
		return;
	}
	UWidgetAnimLib::PlayAlphaFlashAnim(m_ImgTakeHit, 1.f);
}

void UWidgetScreenEffect::HideFadeOut()
{
	if(!IsFadeOut)
	{
		return;
	}
	EndFade();
}
////////////////////////////////////////


void UWidgetAnimLib::PlayTextAnim(UTextBlock* textblock, FLinearColor colorWant, float dur)
{
	textblock->SetColorAndOpacity(colorWant);
	UBUITween::Create(textblock, dur)
		.FromScale(FVector2D(1.0f, 1.0f))
		.ToScale(FVector2D(1.5f, 1.5f))
		.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
		{
			UTextBlock* OwnerT = Cast<UTextBlock>(Owner);
			if(OwnerT)
			{
				RecoverTextAnim(OwnerT, 0.2f);
			}
		}))
		.Begin();
}



void UWidgetAnimLib::RecoverTextAnim(UTextBlock* textblock, float dur)
{
	textblock->SetColorAndOpacity(FLinearColor::White);
	UBUITween::Create(textblock, dur)
		.FromScale(FVector2D(1.5f, 1.5f))
		.ToScale(FVector2D(1.0f, 1.0f))
		.OnComplete(FBUITweenSignature::CreateLambda([](UWidget* Owner)
		{
			
		}))
		.Begin();
}

FBUITweenInstance& UWidgetAnimLib::PlayAlphaFlashAnim(UImage* textblock, float dur)
{
	float Dur1 = dur * 0.2f;
	float Dur2 = dur * 0.8f;
	
	FBUITweenInstance& Tween = UBUITween::Create(textblock, Dur1)
							.FromOpacity(0.f)
							.ToOpacity(1.f)
							.OnComplete(FBUITweenSignature::CreateLambda([=](UWidget* Owner)
							{
								UImage* OwnerT = Cast<UImage>(Owner);
								if (OwnerT)
								{
									RecoverAlphaFlashAnim(OwnerT, Dur2);
								}
							}));

	Tween.Begin();
	return Tween;
}

void UWidgetAnimLib::RecoverAlphaFlashAnim(UImage* textblock, float dur)
{
	UBUITween::Create(textblock, dur)
		.FromOpacity(1.f)
		.ToOpacity(0.f)
		
	.Begin();
}
