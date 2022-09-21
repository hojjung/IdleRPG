#include "Avatar/Widget/WidgetAvatarPanel.h"

void UWidgetAvatarPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetAvatarPanel::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
