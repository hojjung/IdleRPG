#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipMenuEle.h"

#include "Entity.h"

void UWidgetEquipMenuEle::SetEquipData(const FEquipRow& dataEquip, const FOnClick& onClick)
{
	m_Row = &dataEquip;

	m_OnClick = onClick;

	const FColorDataRow& Color = *m_Row->m_Color.GetRow<FColorDataRow>("");

	m_ImgGlow->SetColorAndOpacity(Color.m_Color.GetSpecifiedColor());

	m_ImgTier->SetBrushFromTexture(Color.m_GlowTexture);

	m_ImgIcon->SetBrushFromTexture(dataEquip.m_Icon);
	
	SetEquipSpec(0, false, 0);

	m_nSortOrder = (Color.m_fPriority * 1000) + dataEquip.m_nTier;

	FString Str = FString::Printf(TEXT("T%d"), dataEquip.m_nTier);
	
	m_TextTierLevel->SetText(FText::FromString(Str));
}

int UWidgetEquipMenuEle::GetSortOrder() const
{
	return m_nSortOrder;
}

void UWidgetEquipMenuEle::SetEquipSpec(int level, bool isEquip, int amount)
{
	if(isEquip)
	{
		m_SizeEquip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_SizeEquip->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(level > 0)
	{
		m_TextLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		FString Str = FString::Printf(TEXT("+%d"), level);
		
		m_TextLevel->SetText(FText::FromString(Str));
	}
	else
	{
		m_TextLevel->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	FString Str = FString::Printf(TEXT("%d/5"), amount);
		
	m_TextCount->SetText(FText::FromString(Str));
}

FReply UWidgetEquipMenuEle::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	m_OnClick.ExecuteIfBound(*m_Row);

	return FReply::Handled();
}

