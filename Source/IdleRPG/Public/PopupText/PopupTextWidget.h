// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PopupTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UPopupTextWidget : public UObject
{
	GENERATED_BODY()
	
};


class IDLERPG_API SPopupText : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPopupText)
	{
		_Clipping = EWidgetClipping::Inherit;
	}
	
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
 
	TSharedPtr<SImage> m_ImageSlate;

	TSharedPtr<STextBlock> m_TextSlate;

public:
	void SetTextColorAndOpacity( const TAttribute<FSlateColor>& InColorAndOpacity );
	
	void SetTextColorAndOpacity(FLinearColor InColorAndOpacity );

	void SetImageColorAndOpacity( const TAttribute<FSlateColor>& InColorAndOpacity );
	
	void SetImageColorAndOpacity(FLinearColor InColorAndOpacity );

	void SetImage(TAttribute<const FSlateBrush*> InImage);

	void SetText( const TAttribute< FText >& InText );
	
	void SetText( const FText& InText );
	
	void SetFont(const TAttribute< FSlateFontInfo >& InFont);

	void SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush);

	void SetTextStyle(const FTextBlockStyle* InTextStyle);

	void SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod);

	void SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection);

	void SetWrapTextAt(const TAttribute<float>& InWrapTextAt);

	void SetAutoWrapText(const TAttribute<bool>& InAutoWrapText);

	void SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy);

	void SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset);

	void SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity);

	void SetMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth);

	void SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage);

	void SetMargin(const TAttribute<FMargin>& InMargin);

	void SetJustification(const TAttribute<ETextJustify::Type>& InJustification);
};
