#include "Widgets/GameLevel/HUD/WidgetDeadAlert.h"
#include "Widgets/GameLevel/WidgetMainCanvas.h"

void UWidgetDeadAlert::Init(UWidgetMainCanvas* parent)
{
	m_ParentCanvas = parent;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	
}

void UWidgetDeadAlert::OnOpenUpgrade()
{
	m_ParentCanvas->OnOpenUpgradePanel();
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetDeadAlert::OnOpenSkill()
{
	m_ParentCanvas->OnClickSkillPanel();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetDeadAlert::OnOpenAcce()
{
	m_ParentCanvas->OnClickAccePanel();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetDeadAlert::OnOpenRelic()
{
	m_ParentCanvas->OnClickRelicPanel();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetDeadAlert::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);
	
	m_BtnUpgrade->OnClicked.AddDynamic(this, &UWidgetDeadAlert::OnOpenUpgrade);
	m_BtnSkill->OnClicked.AddDynamic(this, &UWidgetDeadAlert::OnOpenSkill);
	m_BtnAcce->OnClicked.AddDynamic(this, &UWidgetDeadAlert::OnOpenAcce);
	m_BtnRelic->OnClicked.AddDynamic(this, &UWidgetDeadAlert::OnOpenRelic);
}

FReply UWidgetDeadAlert::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply r = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	SetVisibility(ESlateVisibility::Collapsed);

	return  FReply::Handled();
}
