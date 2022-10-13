// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillPanel.h"

#include "Entity.h"
#include "Skill/SkillData.h"

void UWidgetSkillPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Init(NSLOCTEXT("UWidgetSkillPanel","SkillPanel","스킬"), USkillData::GetSkillData, 2000);
	
}
void UWidgetSkillPanel::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_GachaBtn->SetIcon(m_GachaIcon);
}

void UWidgetSkillPanel::Init(FText panelName, const UDataTable* equipItems, int costPerOne)
{
	m_SkillInfo->Close();
	
	m_GachaBtn->Init(panelName, costPerOne);

	equipItems->ForeachRow<FSkillDataRow>("",[&](const FName& key, const FSkillDataRow& row)
	{
		UWidgetSkillEle* Ele = CreateWidget<UWidgetSkillEle>(this, m_ClassEle);
		
		Ele->SetDataOnClick(key, row, UWidgetItemEle::FOnClick::CreateUObject(this, &UWidgetSkillPanel::OnSelectSkill));

		m_AryEles.Add(Ele);
	});

	Sort();
}

void UWidgetSkillPanel::OnSelectSkill(const FName& id , const FEntityDataRow& row)
{
	PRINTF("Show Info");
	m_SkillInfo->ShowInfo((const FSkillDataRow&)row);
}

void UWidgetSkillPanel::Sort()
{
	m_AryEles.Sort([](const UWidgetSkillEle& LHS, const UWidgetSkillEle& RHS)
	{
		int LhsLevel = LHS.GetSortOrder();

		int RhsLevel = RHS.GetSortOrder();

		return LhsLevel < RhsLevel;
	});

	int RowY = 0;

	int ColumnX = 0;
	
	for(UWidgetSkillEle* Ele : m_AryEles)
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

void UWidgetSkillPanel::UpdatePanel()
{
	//매니저가 가진 실제 데이터와 배열 포문 돌려서 장착 및 머지 가능 여부 필요
}



void UWidgetSkillPanel::OnShow()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);	
}

void UWidgetSkillPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
	m_SkillInfo->Close();
}

