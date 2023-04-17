// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/HUD/WidgetHUDPlayerInfo.h"

#include "BUITween.h"
#include "Manager/MyGameInstance.h"

void UWidgetHUDPlayerInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnMenu->OnClicked.AddDynamic(this, &UWidgetHUDPlayerInfo::ToggleMenu);

	BindGold();

	BindAvatar();
}



void UWidgetHUDPlayerInfo::BindGold()
{
	UMyGameInstance::Get->m_GoldManager->m_OnCurrencyChanged.AddUObject(this, &UWidgetHUDPlayerInfo::UpdateGold);

	UpdateGold();
}

void UWidgetHUDPlayerInfo::UpdateGold()
{
	BigInt Gold = UMyGameInstance::Get->m_GoldManager->GetCurrentGold();

	FText GoldText = FText::FromString(UBigIntLib::GetAlphabetTextBigInt(Gold));

	m_Gold->SetText(GoldText);
}

void UWidgetHUDPlayerInfo::BindAvatar()
{
	UMyGameInstance::Get->m_AvatarManager->m_AvatarInven->m_OnSkinChanged.AddUObject(this, &UWidgetHUDPlayerInfo::SetData);

	FName KeyEquip = UMyGameInstance::Get->m_AvatarManager->m_AvatarInven->GetKeySkin();

	const FAvatarRow& RowEquip = UMyGameInstance::Get->m_AvatarManager->m_AvatarInven->GetRowSkin();

	SetData(KeyEquip, RowEquip);
}

void UWidgetHUDPlayerInfo::SetData(const FName& KeyEquip, const FAvatarRow& row)
{
	m_Avatar->SetData(KeyEquip, row);
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
