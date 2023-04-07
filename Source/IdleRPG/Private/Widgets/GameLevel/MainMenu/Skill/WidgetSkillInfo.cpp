#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillInfo.h"

#include "Manager/MyGameInstance.h"
#include "Skill/Active/SkillActiveBase.h"
#include "Skill/Passive/SkillPassiveBase.h"
#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipMenuEle.h"

void UWidgetSkillInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetSkillInfo::Close);
	m_BtnLevelUp->OnClicked.AddDynamic(this, &UWidgetSkillInfo::LevelUp);
	m_BtnEquip->OnClicked.AddDynamic(this, &UWidgetSkillInfo::Equip);

	m_SkillQuick->GetParent()->SetVisibility(ESlateVisibility::Collapsed);
	
	m_SkillQuick->HideAutoButton();
	m_SkillQuick->RegisterMode(this);
}

void UWidgetSkillInfo::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);

	m_SkillQuick->GetParent()->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetSkillInfo::LevelUp()
{
	const FName& ID = m_ItemEle->GetSelectedID();
	
	UMyGameInstance::Get->m_SkillInven->LevelUpSkill(ID);
}

void UWidgetSkillInfo::Equip()
{
	m_SkillQuick->GetParent()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	
	//const FName& ID = m_ItemEle->GetSelectedID();
	//UMyGameInstance::Get->m_SkillInven->EquipSkill(ID);
}

void UWidgetSkillInfo::ShowInfo(const FName& id , const FSkillDataRow& row)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextName->SetText(row.m_Name);

	const FColorDataRow& Color = *row.m_Color.GetRow<FColorDataRow>("");
	
	m_TextTier->SetText(Color.m_Name);

	m_TextTier->SetColorAndOpacity(Color.m_Color);

	const FSkillInven& SkillInvenData = UMyGameInstance::Get->m_SkillInven->GetMapSkillInven()[id];

	FString Str = FString::Printf(TEXT("Lv.%d"),SkillInvenData.m_nLevel);
	
	m_TextLevel->SetText(FText::FromString(Str));

	m_ItemEle->SetData(id, row);

	m_ItemEle->SetLevel(SkillInvenData.m_nLevel);

	m_ItemEle->SetAmount(0);

	USkillBase* SkillBase = row.m_ClassSkillBase->GetDefaultObject<USkillBase>();

	USkillActiveBase* ActiveSkill = Cast<USkillActiveBase>(SkillBase);
	
	if(!ActiveSkill)
	{
		m_TextCooltime->SetText(NSLOCTEXT("UWidgetSkillInfo","NoCooltimeFormat","대기시간 -"));
	}
	else
	{
		float Cd = row.m_fCooltime;

		FText CooltimeFormatText = FText::Format(NSLOCTEXT("UWidgetSkillInfo","CooltimeFormat","대기시간 {0}초"), Cd);

		m_TextCooltime->SetText(CooltimeFormatText);	
	}
	
	FText DescFormatText = SkillBase->GetDescString(SkillInvenData.m_nLevel);
	
	m_TextDesc->SetText(DescFormatText);
}

void UWidgetSkillInfo::EquipSelected(int ndx)
{
	const FName& Id = m_ItemEle->GetSelectedID();
	
	UMyGameInstance::Get->m_SkillInven->EquipSkill(Id, ndx);
}
