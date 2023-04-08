// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillQuickEle.h"
#include "Blueprint/WidgetTree.h"
#include "Manager/MyGameInstance.h"
#include "Widgets/GameLevel/MainMenu/Skill/WidgetSkillInfo.h"

void UWidgetSkillQuickEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_bIsRegisterMode = false;

	m_ImgSkillIcon->SetVisibility(ESlateVisibility::Collapsed);

	m_ImgSkillAdd->SetVisibility(ESlateVisibility::Visible);

	m_ImgCD->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Value"),0);

	m_TextCd->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UWidgetSkillQuickEle::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
	
	if(m_bIsRegisterMode)
	{
		m_OnClick.ExecuteIfBound(m_nIndex);
	}
	else if(m_SkillInst)
	{
		UseSkill();
	}

	return FReply::Handled();
}

void UWidgetSkillQuickEle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCoolTime();
}

void UWidgetSkillQuickEle::UpdateCoolTime()
{
	if(!m_SkillInst || m_bIsRegisterMode)
	{
		return;
	}
	
	float CoolPercent = m_SkillInst->m_fCooldownRemain / m_fMaxCooldown;

	float CoolDiff = m_SkillInst->m_fCooldownRemain;

	m_ImgCD->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Value"),CoolPercent);

	if(CoolDiff < 0)
	{
		m_TextCd->SetText(FText());

		return;
	}
	
	FString StrCd = FString::Printf(TEXT("%.0f"), CoolDiff); 

	m_TextCd->SetText(FText::FromString(StrCd));	
}

void UWidgetSkillQuickEle::UseSkill()
{
	UMyGameInstance::Get->m_SkillInven->UseSkill(m_nIndex);
}

void UWidgetSkillQuickEle::SetIndex(int ndx)
{
	m_nIndex = ndx;
}

void UWidgetSkillQuickEle::SetRegisterMode()
{
	m_bIsRegisterMode = true;
}

void UWidgetSkillQuickEle::EquipSkill()
{
	m_ImgSkillAdd->SetVisibility(ESlateVisibility::Collapsed);

	m_ImgSkillIcon->SetVisibility(ESlateVisibility::Visible);

	m_TextCd->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextCd->SetText(FText());
}

void UWidgetSkillQuickEle::UnequipSkill()
{
	m_ImgSkillAdd->SetVisibility(ESlateVisibility::Visible);

	m_ImgSkillIcon->SetVisibility(ESlateVisibility::Collapsed);

	m_TextCd->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetSkillQuickEle::Update(const FSkillInven* skillInst)
{
	m_SkillInst = skillInst;

	if(m_SkillInst)
	{
		EquipSkill();
		
		const FSkillDataRow* Data = USkillData::GetSkillData->FindRow<FSkillDataRow>(m_SkillInst->m_SkillID, "");
		
		m_ImgSkillIcon->SetBrushFromTexture(Data->m_Icon);

		m_fMaxCooldown = Data->m_fCooltime;

		
	}
	else
	{
		UnequipSkill();
		
	}
}
