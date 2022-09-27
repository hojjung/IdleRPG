// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/WidgetUpgradeEle.h"

void UWidgetUpgradeEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidgetUpgradeEle::SetUpgradeData(const FUpgradeDataTableRow& row)
{
	m_Row = &row;

	m_TextMaxLevel->SetText(FText::AsNumber(row.m_nMaxLevel));
	
	m_ImgIcon->SetBrushFromTexture(m_Row->m_Icon);

	SetLevel(0);
}

void UWidgetUpgradeEle::SetLevel(int lv)
{
	//FString FormatStr = FString::Printf(TEXT("%d"), lv);
	
	m_TextDesc->SetText(FText::FromString(m_Row->m_StrUpgradeDesc));
}
