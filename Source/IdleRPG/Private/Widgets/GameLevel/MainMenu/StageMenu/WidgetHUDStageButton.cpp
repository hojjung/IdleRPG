#include "Widgets/GameLevel/MainMenu/StageMenu/WidgetHUDStageButton.h"

void UWidgetHUDStageButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_MapChanged.AddUObject(this, &UWidgetHUDStageButton::UpdateText);

	UpdateText();
}

void UWidgetHUDStageButton::UpdateText()
{
	const FText& n = UMyGameInstance::Get->GetStageMode()->GetStageName();

	m_TextStageName->SetText(n);
}

UButton* UWidgetHUDStageButton::GetBtn()
{
	return m_BtnOpenStage;
}
