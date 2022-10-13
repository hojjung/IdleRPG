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

void UWidgetEquipMenuEle::UpdateSpec()
{
	bool isEquip = false;
	int level = 0;
	int amount = 0;
	
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
		m_TextCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		FString Str = FString::Printf(TEXT("+%d"), level);
		
		m_TextCount->SetText(FText::FromString(Str));
	}
	else
	{
		m_TextCount->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	FString Str = FString::Printf(TEXT("%d/5"), amount);
		
	m_TextMergeCount->SetText(FText::FromString(Str));
}