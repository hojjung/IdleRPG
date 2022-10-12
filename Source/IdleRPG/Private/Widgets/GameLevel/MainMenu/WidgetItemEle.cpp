#include "Widgets/GameLevel/MainMenu/WidgetItemEle.h"
#include "DataTableRow/ColorData.h"

void UWidgetItemEle::SetData(const FName& id, const FEntityDataRow& dataEquip, FOnClick onClick)
{
	m_Row = &dataEquip;

	m_OnClick = onClick;

	const FColorDataRow& Color = *m_Row->m_Color.GetRow<FColorDataRow>("");

	m_ImgGlow->SetColorAndOpacity(Color.m_Color.GetSpecifiedColor());

	m_ImgTier->SetBrushFromTexture(Color.m_GlowTexture);

	m_ImgIcon->SetBrushFromTexture(dataEquip.m_Icon);
	
	m_nSortOrder = (Color.m_fPriority * 1000) + dataEquip.m_fPriority;

	SetHideTopText();
	
	SetHideBottomText();
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

FReply UWidgetItemEle::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	m_OnClick.ExecuteIfBound(m_ID, *m_Row);

	return FReply::Handled();
}

void UWidgetItemEle::NativeDestruct()
{
	Super::NativeDestruct();

	m_Row = nullptr;
}

void UWidgetItemEle::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_SizeEle->SetWidthOverride(m_Size.X);
	m_SizeEle->SetHeightOverride(m_Size.Y);
}

