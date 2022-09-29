// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetClassPanel.h"

void UWidgetClassPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UClassUpData::GetUpgradeTable->ForeachRow<FClassUpDataTableRow>("",[=](const FName& key, const FClassUpDataTableRow& row)
	{
		UWidgetClassEle* Ele = CreateWidget<UWidgetClassEle>(this, m_ClassEle);
		Ele->SetClassData(row);
		m_ScrollBox->AddChild(Ele);
	});
}

void UWidgetClassPanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
