// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Components/TextWidgetTypes.h"
#include "Components/Widget.h"
#include "Engine/StreamableManager.h"
#include "Slate/SlateBrushAsset.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/SlateTextBlockLayout.h"
#include "Widgets/Text/STextBlock.h"

#include "ImageText.generated.h"


class CUSTOMSLATES_API SImageText : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImageText)
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

UCLASS(BlueprintType)
class CUSTOMSLATES_API UWidgetImageText : public UWidget
{
	GENERATED_BODY()
public:
	UWidgetImageText(const FObjectInitializer& objInit);
	
public:
	TSharedPtr<SImageText> MyImageText;
	
	TSharedPtr<FStreamableHandle> StreamingHandle;
	
	FSoftObjectPath StreamingObjectPath;
	
public:
	void RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, TFunction<void()>&& Callback);
	
	virtual void RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, FStreamableDelegate DelegateToCall);
	
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void SynchronizeProperties() override;

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

public:
	
	public:
	/** The text to display */
	UPROPERTY(EditAnywhere, Category=Content, meta=( MultiLine="true" ))
	FText Text;
	
	/** A bindable delegate to allow logic to drive the text of the widget */
	UPROPERTY()
	FGetText TextDelegate;

	/** The color of the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( sRGB="true") )
	FSlateColor TextColorAndOpacity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( sRGB="true") )
	FLinearColor ImageColorAndOpacity;

	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
	FGetSlateColor TextColorAndOpacityDelegate;
	UPROPERTY()
	FGetLinearColor ImageColorAndOpacityDelegate;
	
	/** The font to render the text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateFontInfo Font;

	/** The brush to strike through text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush StrikeBrush;

	/** The direction the shadow is cast */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FVector2D ShadowOffset;

	/** The color of the shadow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, meta=( DisplayName="Shadow Color" ))
	FLinearColor ShadowColorAndOpacity;

	/** A bindable delegate for the ShadowColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor ShadowColorAndOpacityDelegate;

	/** The minimum desired size for the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float MinDesiredWidth;

	/** If true, it will automatically wrap this text widget with an invalidation panel */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Performance, AdvancedDisplay)
	bool bWrapWithInvalidationPanel;

	/** Whether the text should automatically wrap */
	
	/** Image to draw */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Brush;

	/** A bindable delegate for the Image. */
	UPROPERTY()
	FGetSlateBrush BrushDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Localization, AdvancedDisplay, meta=(ShowOnlyInnerProperties))
	FShapedTextOptions ShapedTextOptions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetJustification, Category=Appearance)
	TEnumAsByte<ETextJustify::Type> Justification;

	/** The wrapping policy to use. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping, AdvancedDisplay)
	ETextWrappingPolicy WrappingPolicy;

	/** True if we're wrapping text automatically based on the computed horizontal space for this widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping)
	uint8 AutoWrapText:1;

	/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Wrapping)
	float WrapTextAt;

	/** The amount of blank space left around the edges of text area. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	FMargin Margin;

	/** The amount to scale each lines height by. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	float LineHeightPercentage;

	/** Color and opacity */
	/** Flips the image if the localization's flow direction is RightToLeft */
	UPROPERTY(EditAnywhere, Category = "Localization")
	bool bFlipForRightToLeftFlowDirection;

public:
	PROPERTY_BINDING_IMPLEMENTATION(FText, Text);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, TextColorAndOpacity);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ImageColorAndOpacity);
	PROPERTY_BINDING_IMPLEMENTATION(FLinearColor, ShadowColorAndOpacity);

	
public:
	UFUNCTION(BlueprintSetter)
	virtual void SetJustification(ETextJustify::Type InJustification);// {  }
	
	TAttribute<FText> GetDisplayText();
	
	void CancelImageStreaming();

	UFUNCTION(BlueprintCallable, Category="Widget", meta=(DisplayName="SetText (Text)"))
	virtual void SetText(FText InText);
	
	UFUNCTION(BlueprintCallable, Category="Appearance")
	void SetImageColorAndOpacity(FLinearColor InColorAndOpacity);
	
	UFUNCTION(BlueprintCallable, Category="Appearance")
	void SetTextColorAndOpacity(FLinearColor InColorAndOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	void SetImageOpacity(float InOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushSize(FVector2D DesiredSize);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushTintColor(FSlateColor TintColor);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushResourceObject(UObject* ResourceObject);
	
	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	virtual void SetBrush(const FSlateBrush& InBrush);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	virtual void SetBrushFromAsset(USlateBrushAsset* Asset);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	virtual void SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	virtual void SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	virtual void SetBrushFromMaterial(UMaterialInterface* Material);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	virtual void SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	virtual void SetBrushFromSoftMaterial(TSoftObjectPtr<UMaterialInterface> SoftMaterial);

	/**  */
	UFUNCTION(BlueprintCallable, Category="Appearance")
	UMaterialInstanceDynamic* GetDynamicMaterial();

	const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;

	void SynchronizeTextLayoutProperties();
};
