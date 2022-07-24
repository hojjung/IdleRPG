#include "PopupText/PopupTextWComp.h"

#include "PopupText/PopupTextWidget.h"

UPopupTextWComp::UPopupTextWComp()
{
	SetDrawAtDesiredSize(true);
	SetPivot(FVector2D (0.5f, 0.5f));
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCanEverAffectNavigation(false);
	SetManuallyRedraw(true);
}

void UPopupTextWComp::BeginPlay()
{
	Super::BeginPlay();

	SAssignNew(m_PopupText, SPopupText);
	
	SetSlateWidget(m_PopupText);
	
	m_PopupText->SetVisibility(TAttribute<EVisibility>(EVisibility::Collapsed));
}

void UPopupTextWComp::SetTextWant(const FVector& loc, const FText& text, int index)
{
	SetWorldLocation(loc);

	//m_PopupText->SetTextWant(textWant,dmg);
}
