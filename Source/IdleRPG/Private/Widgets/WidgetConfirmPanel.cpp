// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WidgetConfirmPanel.h"


void UWidgetConfirmPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	m_BtnConfirm->OnClicked.AddDynamic(this, &UWidgetConfirmPanel::OnConfirm);
	m_BtnCancel->OnClicked.AddDynamic(this, &UWidgetConfirmPanel::OnCancel);
}

void UWidgetConfirmPanel::SetConfirmPanel(const FString&& str, FOnClick onCancel, FOnClick onConfirm)
{
	SetConfirmPanel(str,onCancel,onConfirm);
}

void UWidgetConfirmPanel::SetConfirmPanel(const FString& str, FOnClick onCancel, FOnClick onConfirm)
{
	SetConfirmPanel(FText::FromString(str),onCancel,onConfirm);
}
void UWidgetConfirmPanel::SetConfirmPanel(const FText&& txt, FOnClick onCancel, FOnClick onConfirm)
{
	SetConfirmPanel(txt,onCancel,onConfirm);
}
void UWidgetConfirmPanel::SetConfirmPanel(const FText& txt, FOnClick onCancel, FOnClick onConfirm)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextDesc->SetText(txt);

	m_OnCancel = onCancel;

	m_OnConfirm = onConfirm;
}

void UWidgetConfirmPanel::OnCancel()
{
	m_OnCancel.ExecuteIfBound();
	
	SetVisibility(ESlateVisibility::Collapsed);

	m_OnCancel.Unbind();

	m_OnConfirm.Unbind();
}

void UWidgetConfirmPanel::OnConfirm()
{
	m_OnConfirm.ExecuteIfBound();

	SetVisibility(ESlateVisibility::Collapsed);

	m_OnCancel.Unbind();

	m_OnConfirm.Unbind();
}
