// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Relic/WidgetRelicEle.h"

void UWidgetRelicEle::SetRelicData(const FRelicRow& row, const FOnClick& delegate)
{
	m_Row = &row;

	m_OnClick = delegate;
	
	m_BtnUpgrade->OnClicked.AddDynamic(this, &UWidgetRelicEle::OnClickButton);

	const FColorDataRow* FoundColor = m_Row->m_Color.GetRow<FColorDataRow>("");

	m_nSortOrder = (FoundColor->m_fPriority * 1000) + m_Row->m_nTier;

	m_ImgIcon->SetBrushFromTexture(row.m_Icon);

	m_TextRelicName->SetText(row.m_Name);

	FString Str = FString::Printf(TEXT("Max Lv.%d"), m_Row->m_nMaxLevel);
	
	m_TextMaxLevel->SetText(FText::FromString(Str));

	SetRelicSpec(0,0);
}

void UWidgetRelicEle::SetRelicSpec(int currentLevel, int amountHave)
{
	FString Str = FString::Printf(TEXT("Lv.%d"), currentLevel);
	
	m_TextLevel->SetText(FText::FromString(Str));

	Str = FString::Printf(TEXT("%d/1"), amountHave);//max will change
	
	m_TextCount->SetText(FText::FromString(Str));

	FText LocT = NSLOCTEXT("UWidgetRelicEle", "SetRelicSpec", "성공률 ");

	float Percent = (currentLevel / m_Row->m_nMaxLevel) * 100.0f;

	Str = FString::Printf(TEXT("%s%.0f"), *LocT.ToString(), Percent);

	m_TextPercent->SetText(FText::FromString(Str));

	//m_TextDesc
}

void UWidgetRelicEle::OnClickButton()
{
	m_OnClick.ExecuteIfBound(*m_Row);
}

int UWidgetRelicEle::GetSortOrder() const
{
	return m_nSortOrder;	
}
