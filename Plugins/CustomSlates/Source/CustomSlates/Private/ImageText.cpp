#include "ImageText.h"
#include "SlateOptMacros.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "Engine/Font.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Framework/Text/PlainTextLayoutMarshaller.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Types/ReflectionMetadata.h"
#include "UObject/ConstructorHelpers.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SInvalidationPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SImageText::Construct(const FArguments& InArgs)
{
	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .VAlign(VAlign_Fill)
			  .HAlign(HAlign_Left)
			  .AutoWidth()
			[
				SAssignNew(m_ImageSlate, SImage)
			]
			+ SHorizontalBox::Slot()
			  .VAlign(VAlign_Center)
			  .HAlign(HAlign_Right)
			  .AutoWidth()
			  .Padding(0,3,0,0)
			[
				SAssignNew(m_TextSlate, STextBlock)
			]
		];
}



void SImageText::SetTextColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	m_TextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void SImageText::SetTextColorAndOpacity(FLinearColor InColorAndOpacity)
{
	m_TextSlate->SetColorAndOpacity(InColorAndOpacity);
}

void SImageText::SetImageColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	m_ImageSlate->SetColorAndOpacity(InColorAndOpacity);
}

void SImageText::SetImageColorAndOpacity(FLinearColor InColorAndOpacity)
{
	m_ImageSlate->SetColorAndOpacity(InColorAndOpacity);
}

void SImageText::SetImage(TAttribute<const FSlateBrush*> InImage)
{
	m_ImageSlate->SetImage(InImage);
}


void SImageText::SetText(const TAttribute<FText>& InText)
{
	m_TextSlate->SetText(InText);
}

void SImageText::SetText(const FText& InText)
{
	m_TextSlate->SetText(InText);
}

void SImageText::SetFont(const TAttribute<FSlateFontInfo>& InFont)
{
	m_TextSlate->SetFont(InFont);
}

void SImageText::SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush)
{
	m_TextSlate->SetStrikeBrush(InStrikeBrush);
}

void SImageText::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	m_TextSlate->SetTextStyle(InTextStyle);
}

void SImageText::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	m_TextSlate->SetTextShapingMethod(InTextShapingMethod);
}

void SImageText::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	m_TextSlate->SetTextFlowDirection(InTextFlowDirection);
}

void SImageText::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	m_TextSlate->SetWrapTextAt(InWrapTextAt);
}

void SImageText::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	m_TextSlate->SetAutoWrapText(InAutoWrapText);
}

void SImageText::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	m_TextSlate->SetWrappingPolicy(InWrappingPolicy);
}

void SImageText::SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset)
{
	m_TextSlate->SetShadowOffset(InShadowOffset);
}

void SImageText::SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
{
	m_TextSlate->SetShadowColorAndOpacity(InShadowColorAndOpacity);
}

void SImageText::SetMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth)
{
	m_TextSlate->SetMinDesiredWidth(InMinDesiredWidth);
}

void SImageText::SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	m_TextSlate->SetLineHeightPercentage(InLineHeightPercentage);
}

void SImageText::SetMargin(const TAttribute<FMargin>& InMargin)
{
	m_TextSlate->SetMargin(InMargin);
}

void SImageText::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	m_TextSlate->SetJustification(InJustification);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
//////////////////////
///
///
///
///
///
/////////////
///
///

UWidgetImageText::UWidgetImageText(const FObjectInitializer& objInit):Super(objInit)
{
	TextColorAndOpacity =FLinearColor::White;
	ImageColorAndOpacity=FLinearColor::White;
	ImageColorAndOpacity.A=1;

	LineHeightPercentage=1.f;
	//Font.

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());

		FName FontStyle = TEXT("Bold");

		const UFont* FontObj = RobotoFontObj.Object;

		Font = FSlateFontInfo(FontObj, 24,FontStyle);
	}

	Text = FText::FromString(TEXT("Test Text"));
}

void UWidgetImageText::RequestAsyncLoad(TSoftObjectPtr<> SoftObject, TFunction<void()>&& Callback)
{
	RequestAsyncLoad(SoftObject, FStreamableDelegate::CreateLambda(MoveTemp(Callback)));
}

void UWidgetImageText::RequestAsyncLoad(TSoftObjectPtr<> SoftObject, FStreamableDelegate DelegateToCall)
{
	CancelImageStreaming();

	if (UObject* StrongObject = SoftObject.Get())
	{
		DelegateToCall.ExecuteIfBound();
		return; // No streaming was needed, complete immediately.
	}

	TWeakObjectPtr<UWidgetImageText> WeakThis(this);
	StreamingObjectPath = SoftObject.ToSoftObjectPath();
	StreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StreamingObjectPath,
		[WeakThis, DelegateToCall, SoftObject]()
		{
			if (UWidgetImageText* StrongThis = WeakThis.Get())
			{
				// If the object paths don't match, then this delegate was interrupted, but had already been queued for a callback
				// so ignore everything and abort.
				if (StrongThis->StreamingObjectPath != SoftObject.ToSoftObjectPath())
				{
					return; // Abort!
				}

				// Call the delegate to do whatever is needed, probably set the new image.
				DelegateToCall.ExecuteIfBound();
			}
		},
		FStreamableManager::AsyncLoadHighPriority);
}


TSharedRef<SWidget> UWidgetImageText::RebuildWidget()
{
	if (!IsDesignTime())
	{
		TSharedPtr<SWidget> RetWidget = SNew(SInvalidationPanel)
		[
			SAssignNew(MyImageText, SImageText)
		];

		return RetWidget.ToSharedRef();
	}
	else
	{
		MyImageText = SNew(SImageText);
	}
	return MyImageText.ToSharedRef();
}

void UWidgetImageText::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyImageText.Reset();
}

void UWidgetImageText::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FSlateColor> ImageColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ImageColorAndOpacity);

	TAttribute<const FSlateBrush*> ImageBinding = OPTIONAL_BINDING_CONVERT(
		FSlateBrush, Brush, const FSlateBrush*, ConvertImage);

	TAttribute<FSlateColor> TextColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, TextColorAndOpacity);
	//
	TAttribute<FText> TextBinding = GetDisplayText();

	TAttribute<FLinearColor> ShadowColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, ShadowColorAndOpacity);

	if (MyImageText.IsValid())
	{
		MyImageText->SetImage(ImageBinding);

		MyImageText->SetImageColorAndOpacity(ImageColorAndOpacityBinding);

		MyImageText->SetTextColorAndOpacity(TextColorAndOpacity);

		//
		MyImageText->SetText(TextBinding);
		MyImageText->SetFont(Font);
		MyImageText->SetStrikeBrush(&StrikeBrush);
		MyImageText->SetTextColorAndOpacity(TextColorAndOpacityBinding);
		MyImageText->SetShadowOffset(ShadowOffset);
		MyImageText->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
		MyImageText->SetMinDesiredWidth(MinDesiredWidth);

		SynchronizeTextLayoutProperties();
	}
}

void UWidgetImageText::SynchronizeTextLayoutProperties()
{
	ShapedTextOptions.SynchronizeShapedTextProperties(*MyImageText.Get()->m_TextSlate.Get());

	MyImageText->SetJustification(Justification);
	MyImageText->SetAutoWrapText(!!AutoWrapText);
	MyImageText->SetWrapTextAt(WrapTextAt != 0 ? WrapTextAt : TAttribute<float>());
	MyImageText->SetWrappingPolicy(WrappingPolicy);
	MyImageText->SetMargin(Margin);
	MyImageText->SetLineHeightPercentage(LineHeightPercentage);
}

TSharedPtr<SWidget> UWidgetImageText::GetAccessibleWidget() const
{
	return MyImageText;
}

void UWidgetImageText::SetJustification(ETextJustify::Type InJustification)
{
	Justification = InJustification;

	if (MyImageText.IsValid())
	{
		MyImageText->SetJustification(InJustification);
	}
}

TAttribute<FText> UWidgetImageText::GetDisplayText()
{
	return PROPERTY_BINDING(FText, Text);
}

void UWidgetImageText::CancelImageStreaming()
{
	if (StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
		StreamingHandle.Reset();
	}

	StreamingObjectPath.Reset();
}

void UWidgetImageText::SetText(FText InText)
{
	Text = InText;
	TextDelegate.Unbind();
	if (MyImageText.IsValid())
	{
		TAttribute<FText> TextBinding = GetDisplayText();
		MyImageText->SetText(TextBinding);
	}
}

void UWidgetImageText::SetImageColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ImageColorAndOpacity = InColorAndOpacity;
	if (MyImageText.IsValid())
	{
		MyImageText->SetImageColorAndOpacity(ImageColorAndOpacity);
	}
}

void UWidgetImageText::SetImageOpacity(float InOpacity)
{
	ImageColorAndOpacity.A = InOpacity;
	if (MyImageText.IsValid())
	{
		MyImageText->SetImageColorAndOpacity(ImageColorAndOpacity);
	}
}

void UWidgetImageText::SetTextColorAndOpacity(FLinearColor InColorAndOpacity)
{
	TextColorAndOpacity = InColorAndOpacity;
	if (MyImageText.IsValid())
	{
		MyImageText->SetTextColorAndOpacity(TextColorAndOpacity);
	}
}



void UWidgetImageText::SetBrushSize(FVector2D DesiredSize)
{
	if (Brush.ImageSize != DesiredSize)
	{
		Brush.ImageSize = DesiredSize;

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushTintColor(FSlateColor TintColor)
{
	if (Brush.TintColor != TintColor)
	{
		Brush.TintColor = TintColor;

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushResourceObject(UObject* ResourceObject)
{
	if (Brush.GetResourceObject() != ResourceObject)
	{
		Brush.SetResourceObject(ResourceObject);

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrush(const FSlateBrush& InBrush)
{
	if (Brush != InBrush)
	{
		Brush = InBrush;

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushFromAsset(USlateBrushAsset* Asset)
{
	if (!Asset || Brush != Asset->Brush)
	{
		CancelImageStreaming();
		Brush = Asset ? Asset->Brush : FSlateBrush();

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize)
{
	CancelImageStreaming();

	if (Brush.GetResourceObject() != Texture)
	{
		Brush.SetResourceObject(Texture);

		if (Texture) // Since this texture is used as UI, don't allow it affected by budget.
		{
			Texture->bForceMiplevelsToBeResident = true;
			Texture->bIgnoreStreamingMipBias = true;
		}

		if (bMatchSize)
		{
			if (Texture)
			{
				Brush.ImageSize.X = Texture->GetSizeX();
				Brush.ImageSize.Y = Texture->GetSizeY();
			}
			else
			{
				Brush.ImageSize = FVector2D(0, 0);
			}
		}

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion,
                                                  bool bMatchSize)
{
	if (Brush.GetResourceObject() != AtlasRegion.GetObject())
	{
		CancelImageStreaming();
		Brush.SetResourceObject(AtlasRegion.GetObject());

		if (bMatchSize)
		{
			if (AtlasRegion)
			{
				FSlateAtlasData AtlasData = AtlasRegion->GetSlateAtlasData();
				Brush.ImageSize = AtlasData.GetSourceDimensions();
			}
			else
			{
				Brush.ImageSize = FVector2D(0, 0);
			}
		}

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize)
{
	if (Brush.GetResourceObject() != Texture)
	{
		CancelImageStreaming();
		Brush.SetResourceObject(Texture);

		if (bMatchSize && Texture)
		{
			Brush.ImageSize.X = Texture->SizeX;
			Brush.ImageSize.Y = Texture->SizeY;
		}

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushFromMaterial(UMaterialInterface* Material)
{
	if (Brush.GetResourceObject() != Material)
	{
		CancelImageStreaming();
		Brush.SetResourceObject(Material);

		//TODO UMG Check if the material can be used with the UI

		if (MyImageText.IsValid())
		{
			MyImageText->SetImage(&Brush);
		}
	}
}

void UWidgetImageText::SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize)
{
	TWeakObjectPtr<UWidgetImageText> WeakThis(this);
	// using weak ptr in case 'this' has gone out of scope by the time this lambda is called

	RequestAsyncLoad(SoftTexture,
	                 [WeakThis, SoftTexture, bMatchSize]()
	                 {
		                 if (UWidgetImageText* StrongThis = WeakThis.Get())
		                 {
			                 ensureMsgf(SoftTexture.Get(), TEXT("Failed to load %s"),
			                            *SoftTexture.ToSoftObjectPath().ToString());
			                 StrongThis->SetBrushFromTexture(SoftTexture.Get(), bMatchSize);
		                 }
	                 }
	);
}

void UWidgetImageText::SetBrushFromSoftMaterial(TSoftObjectPtr<UMaterialInterface> SoftMaterial)
{
	TWeakObjectPtr<UWidgetImageText> WeakThis(this);
	// using weak ptr in case 'this' has gone out of scope by the time this lambda is called

	RequestAsyncLoad(SoftMaterial,
	                 [WeakThis, SoftMaterial]()
	                 {
		                 if (UWidgetImageText* StrongThis = WeakThis.Get())
		                 {
			                 ensureMsgf(SoftMaterial.Get(), TEXT("Failed to load %s"),
			                            *SoftMaterial.ToSoftObjectPath().ToString());
			                 StrongThis->SetBrushFromMaterial(SoftMaterial.Get());
		                 }
	                 }
	);
}

UMaterialInstanceDynamic* UWidgetImageText::GetDynamicMaterial()
{
	UMaterialInterface* Material = NULL;

	UObject* Resource = Brush.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Brush.SetResourceObject(DynamicMaterial);

			if (MyImageText.IsValid())
			{
				MyImageText->SetImage(&Brush);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?

	return NULL;
}

const FSlateBrush* UWidgetImageText::ConvertImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UWidgetImageText* MutableThis = const_cast<UWidgetImageText*>(this);
	MutableThis->Brush = InImageAsset.Get();

	return &Brush;
}
