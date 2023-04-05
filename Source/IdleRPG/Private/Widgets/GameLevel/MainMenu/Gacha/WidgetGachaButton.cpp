#include "Widgets/GameLevel/MainMenu/Gacha/WidgetGachaButton.h"

void UWidgetGachaButton::Init(const FText& text, int cost, FOnButtonClickedEvent onClick)
{
	m_TextSummon->SetText(text);

	m_TextCost->SetText(FText::AsNumber(cost));

	m_BtnSummon->OnClicked = onClick;
}

void UWidgetGachaButtonPanel::Init(const FText& text, int costPerOne)
{
	FString SummonStr = NSLOCTEXT("UWidgetGachaButtonPanel", "Summon", "회 소환").ToString();
		
	FString Str = FString::Printf(TEXT("%s 1%s"), *text.ToString(), *SummonStr);

	FOnButtonClickedEvent Summon1;

	Summon1.AddDynamic(this, &UWidgetGachaButtonPanel::OnSummon1);
	
	m_Summon1->Init(FText::FromString(Str), costPerOne, Summon1);

	Str = FString::Printf(TEXT("%s 11%s"), *text.ToString(), *SummonStr);

	FOnButtonClickedEvent Summon11;

	Summon1.AddDynamic(this, &UWidgetGachaButtonPanel::OnSummon11);
	
	m_Summon2->Init(FText::FromString(Str), costPerOne * 10, Summon11);

	Str = FString::Printf(TEXT("%s 34%s"), *text.ToString(), *SummonStr);

	FOnButtonClickedEvent Summon34;

	Summon34.AddDynamic(this, &UWidgetGachaButtonPanel::OnSummon34);
	
	m_Summon3->Init(FText::FromString(Str), costPerOne * 30, Summon34);

	m_BtnShowPercentInfo->OnClicked.AddDynamic(this, &UWidgetGachaButtonPanel::OnClickPercentInfo);
}

void UWidgetGachaButtonPanel::HideBottom()
{
	m_Btn->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetGachaButtonPanel::UpdateLevel(int level, int cExp, int mExp)
{
	FString Str = FString::Printf(TEXT("Lv.%d"), level);
	
	m_Level->SetText(FText::FromString(Str));

	float PerOne = cExp / mExp;

	m_BarExp->SetPercent(PerOne);
	
	Str = FString::Printf(TEXT("%d/%d"), cExp, mExp);

	m_TextExp->SetText(FText::FromString(Str));
}

void UWidgetGachaButtonPanel::SetIcon(UTexture2D* t)
{
	m_Icon->SetBrushFromTexture(t);
}

void UWidgetGachaButtonPanel::OnSummon1()
{
}

void UWidgetGachaButtonPanel::OnSummon11()
{
}

void UWidgetGachaButtonPanel::OnSummon34()
{
}

void UWidgetGachaButtonPanel::OnClickPercentInfo()
{
	
}
