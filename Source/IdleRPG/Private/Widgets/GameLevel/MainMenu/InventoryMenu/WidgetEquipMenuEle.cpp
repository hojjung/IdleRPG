#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipMenuEle.h"

#include "Entity.h"

void UWidgetEquipMenuEle::SetData(const FName& id, const FEntityDataRow& dataEquip)
{
	Super::SetData(id, dataEquip);

	const FEquipRow& EquipDataFound = (const FEquipRow&)dataEquip;

	FString Str = FString::Printf(TEXT("T%d"), EquipDataFound.m_nTier);
	
	m_TextTierLevel->SetText(FText::FromString(Str));

	UpdateSpec();
}

void UWidgetEquipMenuEle::SetLevel(int lv)
{
	m_nLv = lv;

	UpdateSpec();
}

void UWidgetEquipMenuEle::SetAmount(int am)
{
	m_nAm = am;

	UpdateSpec();
}

void UWidgetEquipMenuEle::UpdateSpec()
{
	bool isEquip = false;
	
	if(isEquip)
	{
		m_SizeEquip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_SizeEquip->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(m_nLv > 0)
	{
		m_TextCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		FString Str = FString::Printf(TEXT("+%d"), m_nLv);
		
		m_TextCount->SetText(FText::FromString(Str));
	}
	else
	{
		m_TextCount->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	FString Str = FString::Printf(TEXT("%d/5"), m_nAm);
		
	m_TextMergeCount->SetText(FText::FromString(Str));
}
