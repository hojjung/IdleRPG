#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillQuickPanel.h"

#include "Components/ScrollBoxSlot.h"
#include "Manager/MyGameInstance.h"
#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillInfo.h"

void UWidgetSkillQuickPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateInitSlots();
}

void UWidgetSkillQuickPanel::CreateInitSlots()
{
	m_ScrollSkills->ClearChildren();

	int Iter = -1;
	
	int Count = UMyGameInstance::Get->m_SkillInven->GetSkillInstInven().Num();
	
	while (++Iter < Count)
	{
		UWidgetSkillQuickEle* Ele = CreateWidget<UWidgetSkillQuickEle>(this, m_ClassSkillQuick);

		Ele->SetIndex(Iter);

		UPanelSlot* SlotNew = m_ScrollSkills->AddChild(Ele);

		UScrollBoxSlot* BoxSlotNew = Cast<UScrollBoxSlot>(SlotNew); 

		BoxSlotNew->SetPadding(FMargin(15,0,0,0));

		BoxSlotNew->SetHorizontalAlignment(HAlign_Center);

		BoxSlotNew->SetVerticalAlignment(VAlign_Center);
	}
}

void UWidgetSkillQuickPanel::HideAutoButton()
{
	m_BtnAuto->GetParent()->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetSkillQuickPanel::RegisterMode(UWidgetSkillInfo* info)
{
	const TArray<UWidget*>& Children = m_ScrollSkills->GetAllChildren();

	for(UWidget* MyChild : Children)
	{
		UWidgetSkillQuickEle* MyEle = Cast<UWidgetSkillQuickEle>(MyChild);

		MyEle->SetRegisterMode();

		MyEle->m_OnClick.BindUObject(info, &UWidgetSkillInfo::EquipSelected);
	}
}
