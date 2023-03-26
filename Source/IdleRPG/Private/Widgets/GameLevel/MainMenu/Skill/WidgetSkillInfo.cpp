#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillInfo.h"

#include "Manager/MyGameInstance.h"
#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipMenuEle.h"

void UWidgetSkillInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetSkillInfo::Close);
}

void UWidgetSkillInfo::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetSkillInfo::ShowInfo(const FName& id , const FSkillDataRow& row)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextName->SetText(row.m_Name);

	m_TextTier->SetText(row.m_Color.GetRow<FColorDataRow>("")->m_Name);


	const FSkillInven& SkillInvenData = UMyGameInstance::Get->m_SkillManager->m_SkillInven->GetMapSkillInven()[id];

	FString Str = FString::Printf(TEXT("+%d"),SkillInvenData.m_nLevel);
	
	m_TextLevel->SetText(FText::FromString(Str));

	m_ItemEle->SetData(id, row);

	m_ItemEle->SetLevel(SkillInvenData.m_nLevel);

	m_ItemEle->SetAmount(0);
}
