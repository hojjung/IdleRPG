#include "Widgets/GameLevel/MainMenu/Acce/WidgetAcceMenu.h"
#include "Manager/MyGameInstance.h"

void UWidgetAcceMenu::OnOpen()
{
	Super::OnOpen();

	if(UMyGameInstance::Get->GetPlayerPawn())
	{
		UMyGameInstance::Get->GetPlayerPawn()->SetCameraTop();
	}
}

void UWidgetAcceMenu::OnClose()
{
	Super::OnClose();

	if(UMyGameInstance::Get->GetPlayerPawn())
	{
		UMyGameInstance::Get->GetPlayerPawn()->SetCameraCenter();
	}
}
