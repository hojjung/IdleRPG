#include "PopupText/PopupTextWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPopupText::Construct(const FArguments& InArgs)
{
	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(m_TextSlate, STextBlock)
		];
}



void SPopupText::SetTextColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	m_TextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void SPopupText::SetTextColorAndOpacity(FLinearColor InColorAndOpacity)
{
	m_TextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void SPopupText::SetText(const TAttribute<FText>& InText)
{
	m_TextSlate->SetText(InText);
}

void SPopupText::SetText(const FText& InText)
{
	m_TextSlate->SetText(InText);
}

void SPopupText::SetFont(const TAttribute<FSlateFontInfo>& InFont)
{
	m_TextSlate->SetFont(InFont);
}

void SPopupText::SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush)
{
	m_TextSlate->SetStrikeBrush(InStrikeBrush);
}

void SPopupText::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	m_TextSlate->SetTextStyle(InTextStyle);
}

void SPopupText::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	m_TextSlate->SetTextShapingMethod(InTextShapingMethod);
}

void SPopupText::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	m_TextSlate->SetTextFlowDirection(InTextFlowDirection);
}

void SPopupText::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	m_TextSlate->SetWrapTextAt(InWrapTextAt);
}

void SPopupText::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	m_TextSlate->SetAutoWrapText(InAutoWrapText);
}

void SPopupText::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	m_TextSlate->SetWrappingPolicy(InWrappingPolicy);
}

void SPopupText::SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset)
{
	m_TextSlate->SetShadowOffset(InShadowOffset);
}

void SPopupText::SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
{
	m_TextSlate->SetShadowColorAndOpacity(InShadowColorAndOpacity);
}

void SPopupText::SetMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth)
{
	m_TextSlate->SetMinDesiredWidth(InMinDesiredWidth);
}

void SPopupText::SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	m_TextSlate->SetLineHeightPercentage(InLineHeightPercentage);
}

void SPopupText::SetMargin(const TAttribute<FMargin>& InMargin)
{
	m_TextSlate->SetMargin(InMargin);
}

void SPopupText::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	m_TextSlate->SetJustification(InJustification);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION