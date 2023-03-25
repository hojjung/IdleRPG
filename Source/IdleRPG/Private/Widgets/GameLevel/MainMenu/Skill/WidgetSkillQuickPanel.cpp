#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillQuickPanel.h"

#include "Components/ScrollBoxSlot.h"

void UWidgetSkillQuickPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
		
	CreateInitSlots();
}

void UWidgetSkillQuickPanel::CreateInitSlots()
{
	m_ScrollSkills->ClearChildren();
	
	int Iter = -1;
	while (++Iter < m_nMaxSkillCount)
	{
		UWidgetSkillQuickEle* Ele = CreateWidget<UWidgetSkillQuickEle>(this, m_ClassSkillQuick);

		UPanelSlot* SlotNew = m_ScrollSkills->AddChild(Ele);

		UScrollBoxSlot* BoxSlotNew = Cast<UScrollBoxSlot>(SlotNew); 

		BoxSlotNew->SetPadding(FMargin(15,0,0,0));

		BoxSlotNew->SetHorizontalAlignment(HAlign_Center);

		BoxSlotNew->SetVerticalAlignment(VAlign_Center);
	}
}
