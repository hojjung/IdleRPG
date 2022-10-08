#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipMenu.h"

#include "Entity.h"

void UWidgetEquipMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_GachaBtn->SetIcon(m_GachaIcon);
}

void UWidgetEquipMenu::Init(FText panelName, const UDataTable* equipItems, int costPerOne)
{
	m_GachaBtn->Init(panelName, costPerOne);

	equipItems->ForeachRow<FEquipRow>("",[&](const FName& key, const FEquipRow& row)
	{
		UWidgetEquipMenuEle* Ele = CreateWidget<UWidgetEquipMenuEle>(this, m_ClassEle);
		
		Ele->SetEquipData(row, UWidgetEquipMenuEle::FOnClick::CreateUObject(this, &UWidgetEquipMenu::OnSelectEquip));

		m_AryEles.Add(Ele);

		m_Wrap->AddChildToWrapBox(Ele);
	});

	Sort();
}

void UWidgetEquipMenu::Sort()
{
	m_AryEles.Sort([](const UWidgetEquipMenuEle& LHS, const UWidgetEquipMenuEle& RHS)
	{
		int LhsLevel = LHS.GetSortOrder();

		int RhsLevel = RHS.GetSortOrder();

		return LhsLevel < RhsLevel;
	});

	for(UWidgetEquipMenuEle* Ele : m_AryEles)
	{
		m_Wrap->AddChildToWrapBox(Ele);
	}
}

void UWidgetEquipMenu::UpdatePanel()
{
	//매니저가 가진 실제 데이터와 배열 포문 돌려서 장착 및 머지 가능 여부 필요
}

void UWidgetEquipMenu::OnShow()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);	
}

void UWidgetEquipMenu::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}



void UWidgetEquipMenu::OnSelectEquip(const FEquipRow& row)
{
	PRINTF("Show Info");
}
