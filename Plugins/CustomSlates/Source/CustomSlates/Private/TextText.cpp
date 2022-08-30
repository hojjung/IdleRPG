
#include "TextText.h"
#include "SlateOptMacros.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"
#include "Widgets/SInvalidationPanel.h"
#include "Widgets/SOverlay.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STextText::Construct(const FArguments& InArgs)
{
	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .VAlign(VAlign_Fill)
			  .HAlign(HAlign_Left)
			  .FillWidth(1)
			[
				SAssignNew(m_LeftTextSlate, STextBlock)
			]
			+ SHorizontalBox::Slot()
			  .VAlign(VAlign_Fill)
			  .HAlign(HAlign_Right)
			  .FillWidth(1)
			[
				SAssignNew(m_RightTextSlate, STextBlock)
			]
		];
}

void STextText::SetLeftTextColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	m_LeftTextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void STextText::SetLeftTextColorAndOpacity(FLinearColor InColorAndOpacity)
{
	m_LeftTextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void STextText::SetLeftText(const TAttribute<FText>& InText)
{
	m_LeftTextSlate->SetText(InText);
}

void STextText::SetLeftText(const FText& InText)
{
	m_LeftTextSlate->SetText(InText);
}

void STextText::SetLeftFont(const TAttribute<FSlateFontInfo>& InFont)
{
	m_LeftTextSlate->SetFont(InFont);
}

void STextText::SetLeftStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush)
{
	m_LeftTextSlate->SetStrikeBrush(InStrikeBrush);
}

void STextText::SetLeftTextStyle(const FTextBlockStyle* InTextStyle)
{
	m_LeftTextSlate->SetTextStyle(InTextStyle);
}

void STextText::SetLeftTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	m_LeftTextSlate->SetTextShapingMethod(InTextShapingMethod);
}

void STextText::SetLeftTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	m_LeftTextSlate->SetTextFlowDirection(InTextFlowDirection);
}

void STextText::SetLeftWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	m_LeftTextSlate->SetWrapTextAt(InWrapTextAt);
}

void STextText::SetLeftAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	m_LeftTextSlate->SetAutoWrapText(InAutoWrapText);
}

void STextText::SetLeftWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	m_LeftTextSlate->SetWrappingPolicy(InWrappingPolicy);
}

void STextText::SetLeftShadowOffset(const TAttribute<FVector2D>& InShadowOffSetLeft)
{
	m_LeftTextSlate->SetShadowOffset(InShadowOffSetLeft);
}

void STextText::SetLeftShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
{
	m_LeftTextSlate->SetShadowColorAndOpacity(InShadowColorAndOpacity);
}

void STextText::SetLeftMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth)
{
	m_LeftTextSlate->SetMinDesiredWidth(InMinDesiredWidth);
}

void STextText::SetLeftLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	m_LeftTextSlate->SetLineHeightPercentage(InLineHeightPercentage);
}

void STextText::SetLeftMargin(const TAttribute<FMargin>& InMargin)
{
	m_LeftTextSlate->SetMargin(InMargin);
}

void STextText::SetLeftJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	m_LeftTextSlate->SetJustification(InJustification);
}


void STextText::SetRightTextColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	m_RightTextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void STextText::SetRightTextColorAndOpacity(FLinearColor InColorAndOpacity)
{
	m_RightTextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void STextText::SetRightText(const TAttribute<FText>& InText)
{
	m_RightTextSlate->SetText(InText);
}

void STextText::SetRightText(const FText& InText)
{
	m_RightTextSlate->SetText(InText);
}

void STextText::SetRightFont(const TAttribute<FSlateFontInfo>& InFont)
{
	m_RightTextSlate->SetFont(InFont);
}

void STextText::SetRightStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush)
{
	m_RightTextSlate->SetStrikeBrush(InStrikeBrush);
}

void STextText::SetRightTextStyle(const FTextBlockStyle* InTextStyle)
{
	m_RightTextSlate->SetTextStyle(InTextStyle);
}

void STextText::SetRightTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	m_RightTextSlate->SetTextShapingMethod(InTextShapingMethod);
}

void STextText::SetRightTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	m_RightTextSlate->SetTextFlowDirection(InTextFlowDirection);
}

void STextText::SetRightWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	m_RightTextSlate->SetWrapTextAt(InWrapTextAt);
}

void STextText::SetRightAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	m_RightTextSlate->SetAutoWrapText(InAutoWrapText);
}

void STextText::SetRightWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	m_RightTextSlate->SetWrappingPolicy(InWrappingPolicy);
}

void STextText::SetRightShadowOffset(const TAttribute<FVector2D>& InShadowOffSetRight)
{
	m_RightTextSlate->SetShadowOffset(InShadowOffSetRight);
}

void STextText::SetRightShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
{
	m_RightTextSlate->SetShadowColorAndOpacity(InShadowColorAndOpacity);
}

void STextText::SetRightMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth)
{
	m_RightTextSlate->SetMinDesiredWidth(InMinDesiredWidth);
}

void STextText::SetRightLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	m_RightTextSlate->SetLineHeightPercentage(InLineHeightPercentage);
}

void STextText::SetRightMargin(const TAttribute<FMargin>& InMargin)
{
	m_RightTextSlate->SetMargin(InMargin);
}

void STextText::SetRightJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	m_RightTextSlate->SetJustification(InJustification);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


UWidgetTextText::UWidgetTextText(const FObjectInitializer& objInit):Super(objInit)
{
	LeftTextColorAndOpacity =FLinearColor::White;
	LeftLineHeightPercentage=1.f;
	RightTextColorAndOpacity =FLinearColor::White;
	RightLineHeightPercentage=1.f;
	//Font.

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
		
		LeftFont = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
		RightFont = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
	}

	LeftText = FText::FromString(TEXT("Test Text"));
	RightText = FText::FromString(TEXT("Test Text"));
}

TSharedRef<SWidget> UWidgetTextText::RebuildWidget()
{
	if (!IsDesignTime())
	{
		TSharedPtr<SWidget> RetWidget = SNew(SInvalidationPanel)
		[
			SAssignNew(MyTextText, STextText)
		];

		return RetWidget.ToSharedRef();
	}
	else
	{
		MyTextText = SNew(STextText);
	}
	return MyTextText.ToSharedRef();
}

void UWidgetTextText::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
    
    	MyTextText.Reset();
}

void UWidgetTextText::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FSlateColor> LeftTextColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, LeftTextColorAndOpacity);
	TAttribute<FText> LeftTextBinding = GetLeftDisplayText();
	TAttribute<FLinearColor> LeftShadowColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, LeftShadowColorAndOpacity);
	
	TAttribute<FSlateColor> RightTextColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, RightTextColorAndOpacity);
	TAttribute<FText> RightTextBinding = GetRightDisplayText();
	TAttribute<FLinearColor> RightShadowColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, RightShadowColorAndOpacity);

	if (MyTextText.IsValid())
	{
		MyTextText->SetLeftText(LeftTextBinding);
		MyTextText->SetLeftTextColorAndOpacity(LeftTextColorAndOpacityBinding);
		MyTextText->SetLeftShadowColorAndOpacity(LeftShadowColorAndOpacityBinding);
		MyTextText->SetLeftFont(LeftFont);
		MyTextText->SetLeftStrikeBrush(&LeftStrikeBrush);
		MyTextText->SetLeftTextColorAndOpacity(LeftTextColorAndOpacityBinding);
		MyTextText->SetLeftShadowOffset(LeftShadowOffset);
		MyTextText->SetLeftMinDesiredWidth(LeftMinDesiredWidth);
		//
		MyTextText->SetRightText(RightTextBinding);
		MyTextText->SetRightTextColorAndOpacity(RightTextColorAndOpacityBinding);
		MyTextText->SetRightShadowColorAndOpacity(RightShadowColorAndOpacityBinding);
		MyTextText->SetRightFont(RightFont);
		MyTextText->SetRightStrikeBrush(&RightStrikeBrush);
		MyTextText->SetRightTextColorAndOpacity(RightTextColorAndOpacityBinding);
		MyTextText->SetRightShadowOffset(RightShadowOffset);
		MyTextText->SetRightMinDesiredWidth(RightMinDesiredWidth);

		SynchronizeTextLayoutProperties();
	}
}

void UWidgetTextText::SynchronizeTextLayoutProperties()
{
	LeftShapedTextOptions.SynchronizeShapedTextProperties(*MyTextText.Get()->m_LeftTextSlate.Get());
	

	MyTextText->SetLeftJustification(LeftJustification);
	MyTextText->SetLeftAutoWrapText(!!LeftAutoWrapText);
	MyTextText->SetLeftWrapTextAt(LeftWrapTextAt != 0 ? LeftWrapTextAt : TAttribute<float>());
	MyTextText->SetLeftWrappingPolicy(LeftWrappingPolicy);
	MyTextText->SetLeftMargin(LeftMargin);
	MyTextText->SetLeftLineHeightPercentage(LeftLineHeightPercentage);
		//
	RightShapedTextOptions.SynchronizeShapedTextProperties(*MyTextText.Get()->m_RightTextSlate.Get());

	
	MyTextText->SetRightJustification(RightJustification);
	MyTextText->SetRightAutoWrapText(!!RightAutoWrapText);
	MyTextText->SetRightWrapTextAt(RightWrapTextAt != 0 ? RightWrapTextAt : TAttribute<float>());
	MyTextText->SetRightWrappingPolicy(RightWrappingPolicy);
	MyTextText->SetRightMargin(RightMargin);
	MyTextText->SetRightLineHeightPercentage(RightLineHeightPercentage);
}

TSharedPtr<SWidget> UWidgetTextText::GetAccessibleWidget() const
{
	return MyTextText;
}

void UWidgetTextText::SetLeftJustification(ETextJustify::Type InJustification)
{
	LeftJustification = InJustification;

	if (MyTextText.IsValid())
	{
		MyTextText->SetLeftJustification(InJustification);
	}
}

TAttribute<FText> UWidgetTextText::GetLeftDisplayText()
{
	return PROPERTY_BINDING(FText, LeftText);
}


void UWidgetTextText::SetLeftText(FText InText)
{
	LeftText = InText;
	LeftTextDelegate.Unbind();
	if (MyTextText.IsValid())
	{
		TAttribute<FText> TextBinding = GetLeftDisplayText();
		MyTextText->SetLeftText(TextBinding);
	}
}

void UWidgetTextText::SetLeftTextColorAndOpacity(FLinearColor InColorAndOpacity)
{
	LeftTextColorAndOpacity = InColorAndOpacity;
	if (MyTextText.IsValid())
	{
		MyTextText->SetLeftTextColorAndOpacity(LeftTextColorAndOpacity);
	}
}

void UWidgetTextText::SetRightJustification(ETextJustify::Type InJustification)
{
	RightJustification = InJustification;

	if (MyTextText.IsValid())
	{
		MyTextText->SetRightJustification(InJustification);
	}
}

TAttribute<FText> UWidgetTextText::GetRightDisplayText()
{
	return PROPERTY_BINDING(FText, RightText);
}


void UWidgetTextText::SetRightText(FText InText)
{
	RightText = InText;
	RightTextDelegate.Unbind();
	if (MyTextText.IsValid())
	{
		TAttribute<FText> TextBinding = GetRightDisplayText();
		MyTextText->SetRightText(TextBinding);
	}
}

void UWidgetTextText::SetRightTextColorAndOpacity(FLinearColor InColorAndOpacity)
{
	RightTextColorAndOpacity = InColorAndOpacity;
	if (MyTextText.IsValid())
	{
		MyTextText->SetRightTextColorAndOpacity(RightTextColorAndOpacity);
	}
}




