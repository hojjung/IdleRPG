#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"

void UWidgetMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetMenuBase::OnClose);
	OnClose();
}

void UWidgetMenuBase::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetMenuBase::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

bool UWidgetMenuBase::IsOpened()
{
	return IsVisible();
}

