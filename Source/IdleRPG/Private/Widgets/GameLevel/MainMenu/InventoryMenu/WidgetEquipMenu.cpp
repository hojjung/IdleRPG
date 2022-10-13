#include "Widgets/GameLevel/MainMenu/InventoryMenu/WidgetEquipMenu.h"

#include "Entity.h"

void UWidgetEquipMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_GachaBtn->SetIcon(m_GachaIcon);
}

void UWidgetEquipMenu::Init(FText panelName, const UDataTable* equipItems, int costPerOne)
{
	m_EquipInfo->Close();
	
	m_GachaBtn->Init(panelName, costPerOne);

	equipItems->ForeachRow<FEquipRow>("",[&](const FName& key, const FEquipRow& row)
	{
		UWidgetEquipMenuEle* Ele = CreateWidget<UWidgetEquipMenuEle>(this, m_ClassEle);
		
		Ele->SetDataOnClick(key, row, UWidgetItemEle::FOnClick::CreateUObject(this, &UWidgetEquipMenu::OnSelectEquip));

		m_AryEles.Add(Ele);
	});
\
	Sort();
}

void UWidgetEquipMenu::OnSelectEquip(const FName& id , const FEntityDataRow& row)
{
	PRINTF("Show Info");
	m_EquipInfo->ShowInfo((const FEquipRow&)row);
}

void UWidgetEquipMenu::Sort()
{
	m_AryEles.Sort([](const UWidgetEquipMenuEle& LHS, const UWidgetEquipMenuEle& RHS)
	{
		int LhsLevel = LHS.GetSortOrder();

		int RhsLevel = RHS.GetSortOrder();

		return LhsLevel < RhsLevel;
	});

	int RowY = 0;

	int ColumnX = 0;
	
	for(UWidgetEquipMenuEle* Ele : m_AryEles)
	{
		m_Grid->AddChildToUniformGrid(Ele,RowY,ColumnX);

		ColumnX++;

		if(ColumnX >= 5)
		{
			ColumnX = 0;
			RowY++;
		}
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
	m_EquipInfo->Close();
}




