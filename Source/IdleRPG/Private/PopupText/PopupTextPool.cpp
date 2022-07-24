#include "PopupText/PopupTextPool.h"

PopupTextPool::~PopupTextPool()
{
	
}
PopupTextPool::PopupTextPool()
{
	m_CompUseIndex = 0;
}

void PopupTextPool::CreateIGWC(int count, USceneComponent* root)
{
	m_AryWidgets.Reset();

	for (int i = 0; i < count; i++)
	{
		UPopupTextWComp* DamageText = NewObject<UPopupTextWComp>(root->GetOwner(), UPopupTextWComp::StaticClass());
		DamageText->RegisterComponent();
		DamageText->InitializeComponent();
		m_AryWidgets.Add(TStrongObjectPtr<UPopupTextWComp>(DamageText));
		DamageText->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

UPopupTextWComp* PopupTextPool::GetIGWC()
{
	UPopupTextWComp* Dmg = m_AryWidgets[m_CompUseIndex++].Get();

	if (m_CompUseIndex >= m_AryWidgets.Num())
	{
		m_CompUseIndex = 0;
	}
	return Dmg;
}

void PopupTextPool::ShowInGameWorldText(const FString& stringWant, FVector loc, int index)
{
	UPopupTextWComp* TextCompWant = GetIGWC();

	TextCompWant->SetTextWant(loc, FText::FromString(stringWant), index);
}

void PopupTextPool::ShowInGameWorldText(float number, FVector loc, int index)
{
	FString Number =  FString::Printf(TEXT("%.0f"),number);
	ShowInGameWorldText(Number,loc,index);
}