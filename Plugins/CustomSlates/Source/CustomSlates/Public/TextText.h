// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Components/TextWidgetTypes.h"
#include "Components/Widget.h"
#include "Engine/StreamableManager.h"
#include "Slate/SlateBrushAsset.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/SlateTextBlockLayout.h"
#include "Widgets/Text/STextBlock.h"

#include "Widgets/SCompoundWidget.h"
#include "TextText.generated.h"
/**
 * need Generate
 */
class CUSTOMSLATES_API STextText : public SCompoundWidget
{

	public:
	SLATE_BEGIN_ARGS(STextText)
	{
		_Clipping = EWidgetClipping::Inherit;
	}
	
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
 
	TSharedPtr<STextBlock> m_LeftTextSlate;

	TSharedPtr<STextBlock> m_RightTextSlate;

public:
	void SetLeftTextColorAndOpacity( const TAttribute<FSlateColor>& InColorAndOpacity );

	void SetLeftTextColorAndOpacity(FLinearColor InColorAndOpacity );

	void SetLeftText( const TAttribute< FText >& InText );
	
	void SetLeftText( const FText& InText );
	
	void SetLeftFont(const TAttribute< FSlateFontInfo >& InFont);

	void SetLeftStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush);

	void SetLeftTextStyle(const FTextBlockStyle* InTextStyle);

	void SetLeftTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod);

	void SetLeftTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection);

	void SetLeftWrapTextAt(const TAttribute<float>& InWrapTextAt);

	void SetLeftAutoWrapText(const TAttribute<bool>& InAutoWrapText);

	void SetLeftWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy);

	void SetLeftShadowOffset(const TAttribute<FVector2D>& InShadowOffSetLeft);

	void SetLeftShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity);

	void SetLeftMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth);

	void SetLeftLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage);

	void SetLeftMargin(const TAttribute<FMargin>& InMargin);

	void SetLeftJustification(const TAttribute<ETextJustify::Type>& InJustification);

	//right
	void SetRightTextColorAndOpacity( const TAttribute<FSlateColor>& InColorAndOpacity );
	
	void SetRightTextColorAndOpacity(FLinearColor InColorAndOpacity );

	void SetRightText( const TAttribute< FText >& InText );
	
	void SetRightText( const FText& InText );
	
	void SetRightFont(const TAttribute< FSlateFontInfo >& InFont);

	void SetRightStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush);

	void SetRightTextStyle(const FTextBlockStyle* InTextStyle);

	void SetRightTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod);

	void SetRightTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection);

	void SetRightWrapTextAt(const TAttribute<float>& InWrapTextAt);

	void SetRightAutoWrapText(const TAttribute<bool>& InAutoWrapText);

	void SetRightWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy);

	void SetRightShadowOffset(const TAttribute<FVector2D>& InShadowOffSetRight);

	void SetRightShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity);

	void SetRightMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth);

	void SetRightLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage);

	void SetRightMargin(const TAttribute<FMargin>& InMargin);

	void SetRightJustification(const TAttribute<ETextJustify::Type>& InJustification);

};
//


UCLASS(BlueprintType)
class CUSTOMSLATES_API UWidgetTextText : public UWidget
{
	GENERATED_BODY()
public:
	UWidgetTextText(const FObjectInitializer& objInit);
	
public:
	TSharedPtr<STextText> MyTextText;
	
public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;


	virtual void SynchronizeProperties() override;

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

public:
	/** The text to display */
	UPROPERTY(EditAnywhere, Category=Content, meta=( MultiLine="true" ))
	FText LeftText;
	
	/** A bindable delegate to allow logic to drive the text of the widget */
	UPROPERTY()
	FGetText LeftTextDelegate;

	/** The color of the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( sRGB="true") )
	FSlateColor LeftTextColorAndOpacity;
	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
	FGetSlateColor LeftTextColorAndOpacityDelegate;
	/** The font to render the text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateFontInfo LeftFont;

	/** The brush to strike through text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush LeftStrikeBrush;

	/** The direction the shadow is cast */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FVector2D LeftShadowOffset;

	/** The color of the shadow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( DisplayName="Shadow Color" ))
	FLinearColor LeftShadowColorAndOpacity;

	/** A bindable delegate for the ShadowColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor LeftShadowColorAndOpacityDelegate;

	/** The minimum desired size for the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float LeftMinDesiredWidth;

	/** If true, it will automatically wrap this text widget with an invalidation panel */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Performance, AdvancedDisplay)
	bool LeftbWrapWithInvalidationPanel;

	/** Whether the text should automatically wrap */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Localization, AdvancedDisplay, meta=(ShowOnlyInnerProperties))
	FShapedTextOptions LeftShapedTextOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetLeftJustification, Category=Appearance)
	TEnumAsByte<ETextJustify::Type> LeftJustification;

	/** The wrapping policy to use. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping, AdvancedDisplay)
	ETextWrappingPolicy LeftWrappingPolicy;

	/** True if we're wrapping text automatically based on the computed horizontal space for this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping)
	uint8 LeftAutoWrapText:1;

	/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping)
	float LeftWrapTextAt;

	/** The amount of blank space left around the edges of text area. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	FMargin LeftMargin;

	/** The amount to scale each lines height by. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	float LeftLineHeightPercentage;

	/** Color and opacity */
	/** Flips the image if the localization's flow direction is RightToLeft */
	UPROPERTY(EditAnywhere, Category = "Localization")
	bool LeftbFlipForRightToLeftFlowDirection;

public://right
	/** The text to display */
	UPROPERTY(EditAnywhere, Category=Content, meta=( MultiLine="true" ))
	FText RightText;
	
	/** A bindable delegate to allow logic to drive the text of the widget */
	UPROPERTY()
	FGetText RightTextDelegate;

	/** The color of the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( sRGB="true") )
	FSlateColor RightTextColorAndOpacity;
	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
	FGetSlateColor RightTextColorAndOpacityDelegate;
	/** The font to render the text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateFontInfo RightFont;

	/** The brush to strike through text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush RightStrikeBrush;

	/** The direction the shadow is cast */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FVector2D RightShadowOffset;

	/** The color of the shadow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( DisplayName="Shadow Color" ))
	FLinearColor RightShadowColorAndOpacity;

	/** A bindable delegate for the ShadowColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor RightShadowColorAndOpacityDelegate;

	/** The minimum desired size for the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float RightMinDesiredWidth;

	/** If true, it will automatically wrap this text widget with an invalidation panel */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Performance, AdvancedDisplay)
	bool RightbWrapWithInvalidationPanel;

	/** Whether the text should automatically wrap */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Localization, AdvancedDisplay, meta=(ShowOnlyInnerProperties))
	FShapedTextOptions RightShapedTextOptions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetRightJustification, Category=Appearance)
	TEnumAsByte<ETextJustify::Type> RightJustification;

	/** The wrapping policy to use. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping, AdvancedDisplay)
	ETextWrappingPolicy RightWrappingPolicy;

	/** True if we're wrapping text automatically based on the computed horizontal space for this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping)
	uint8 RightAutoWrapText:1;

	/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping)
	float RightWrapTextAt;

	/** The amount of blank space Right around the edges of text area. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	FMargin RightMargin;

	/** The amount to scale each lines height by. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	float RightLineHeightPercentage;

	/** Color and opacity */
	/** Flips the image if the localization's flow direction is RightToRight */
	UPROPERTY(EditAnywhere, Category = "Localization")
	bool RightbFlipForRightToLeftFlowDirection;

public:
	PROPERTY_BINDING_IMPLEMENTATION(FText, LeftText);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, LeftTextColorAndOpacity);
	PROPERTY_BINDING_IMPLEMENTATION(FLinearColor, LeftShadowColorAndOpacity);
	PROPERTY_BINDING_IMPLEMENTATION(FText, RightText);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, RightTextColorAndOpacity);
	PROPERTY_BINDING_IMPLEMENTATION(FLinearColor, RightShadowColorAndOpacity);

	
public:
	UFUNCTION(BlueprintSetter)
	virtual void SetRightJustification(ETextJustify::Type InJustification);// {  }
	
	TAttribute<FText> GetRightDisplayText();
	
	UFUNCTION(BlueprintCallable, Category="Widget", meta=(DisplayName="SetText (Text)"))
	virtual void SetRightText(FText InText);
	
	UFUNCTION(BlueprintCallable, Category="Appearance")
	void SetRightTextColorAndOpacity(FLinearColor InColorAndOpacity);

public:
	UFUNCTION(BlueprintSetter)
	virtual void SetLeftJustification(ETextJustify::Type InJustification);// {  }
	
	TAttribute<FText> GetLeftDisplayText();
	
	UFUNCTION(BlueprintCallable, Category="Widget", meta=(DisplayName="SetText (Text)"))
	virtual void SetLeftText(FText InText);
	
	UFUNCTION(BlueprintCallable, Category="Appearance")
	void SetLeftTextColorAndOpacity(FLinearColor InColorAndOpacity);

	void SynchronizeTextLayoutProperties();
};
