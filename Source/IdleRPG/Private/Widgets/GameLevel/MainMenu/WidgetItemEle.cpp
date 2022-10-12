#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"
#include "DataTableRow/ColorData.h"

void UWidgetItemEle::SetData(const FName& id, const FEntityDataRow& dataEquip, FOnClick onClick)
{
	SetData(id, dataEquip);

	m_OnClick = onClick;
}

void UWidgetItemEle::SetData(const FName& id, const FEntityDataRow& dataEquip)
{
	Clear();
	
	m_ID = id;
	
	m_Row = &dataEquip;

	const FColorDataRow& Color = *m_Row->m_Color.GetRow<FColorDataRow>("");

	SetColorTier(Color);

	SetIcon(dataEquip.m_Icon);
	
	m_nSortOrder = (Color.m_fPriority * 1000) + dataEquip.m_fPriority;

	SetHideTopText();
	
	SetHideBottomText();
}

void UWidgetItemEle::SetColorTier(const FColorDataRow& colorRow)
{
	m_ImgGlow->SetColorAndOpacity(colorRow.m_Color.GetSpecifiedColor());

	m_ImgTier->SetBrushFromTexture(colorRow.m_GlowTexture);
}

void UWidgetItemEle::SetIcon(UTexture2D* icon)
{
	m_ImgIcon->SetBrushFromTexture(icon);
}

void UWidgetItemEle::Clear()
{
	m_bHasTouch = false;
	
	m_Row = nullptr;

	m_ID = NAME_None;

	m_TextTierLevel->SetText(FText());

	m_TextCount->SetText(FText());
}

int UWidgetItemEle::GetSortOrder() const
{
	return m_nSortOrder;
}

void UWidgetItemEle::SetTopText(const FString& str, FLinearColor c)
{
	SetTopText(FText::FromString(str), c);
}

void UWidgetItemEle::SetTopText(const FText& str, FLinearColor c)
{
	m_TextTierLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_TextTierLevel->SetText(str);
	m_TextTierLevel->SetColorAndOpacity(c);
}

void UWidgetItemEle::SetBottomText(const FString& str, FLinearColor c)
{
	SetBottomText(FText::FromString(str), c);
}

void UWidgetItemEle::SetBottomText(const FText& str, FLinearColor c)
{
	m_TextCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_TextCount->SetText(str);
	m_TextCount->SetColorAndOpacity(c);
}

void UWidgetItemEle::SetHideBottomText()
{
	m_TextCount->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetItemEle::SetHideTopText()
{
	m_TextTierLevel->SetVisibility(ESlateVisibility::Collapsed);
}

const FColorDataRow& UWidgetItemEle::GetColorData() const
{
	return *m_Row->m_Color.GetRow<FColorDataRow>("");
}

void UWidgetItemEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nSortOrder = 0;
}

FReply UWidgetItemEle::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	m_bHasTouch = true;
	
	return FReply::Handled();
}

FReply UWidgetItemEle::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	if(m_bHasTouch)
	{
		m_OnClick.ExecuteIfBound(m_ID, *m_Row);
	}

	m_bHasTouch = false;

	return FReply::Handled();
}

void UWidgetItemEle::NativeDestruct()
{
	Super::NativeDestruct();

	Clear();
}

void UWidgetItemEle::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_SizeEle->SetWidthOverride(m_Size.X);
	m_SizeEle->SetHeightOverride(m_Size.Y);
}

