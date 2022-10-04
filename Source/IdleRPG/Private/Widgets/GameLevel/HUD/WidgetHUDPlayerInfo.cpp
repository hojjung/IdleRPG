// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/HUD/WidgetHUDPlayerInfo.h"

#include "BUITween.h"

void UWidgetHUDPlayerInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnMenu->OnClicked.AddDynamic(this, &UWidgetHUDPlayerInfo::ToggleMenu);
}

void UWidgetHUDPlayerInfo::ToggleMenu()
{
	m_BtnMenu->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if(m_WrapboxMenu->IsVisible())
	{
		m_WrapboxMenu->SetVisibility(ESlateVisibility::HitTestInvisible);
		UBUITween::Create(m_WrapboxMenu,0.1f)
		.FromOpacity(1)
		.ToOpacity(0)
		.OnComplete( FBUITweenSignature::CreateLambda([&]( UWidget* Owner )
			{
				Owner->SetVisibility(ESlateVisibility::Collapsed);
				m_BtnMenu->SetVisibility(ESlateVisibility::Visible);
			
			}))
		.Begin();
	}
	else
	{
		m_WrapboxMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UBUITween::Create(m_WrapboxMenu,0.1f)
		.FromOpacity(0)
		.ToOpacity(1)
		.OnComplete( FBUITweenSignature::CreateLambda([&]( UWidget* Owner )
			{
				m_BtnMenu->SetVisibility(ESlateVisibility::Visible);
			
			}))
		.Begin();
	}

}
