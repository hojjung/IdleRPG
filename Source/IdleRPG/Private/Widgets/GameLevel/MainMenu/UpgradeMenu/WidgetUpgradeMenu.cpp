#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetUpgradeMenu.h"

#include "MyGameInstance.h"

void UWidgetUpgradeMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);
	
	m_BtnUpgrade->OnClicked.AddDynamic(this, &UWidgetUpgradeMenu::OpenUpgrade);
	m_BtnLevelStat->OnClicked.AddDynamic(this, &UWidgetUpgradeMenu::OpenLevelStat);
	m_BtnClass->OnClicked.AddDynamic(this, &UWidgetUpgradeMenu::OpenClass);
	m_BtnEmblem->OnClicked.AddDynamic(this, &UWidgetUpgradeMenu::OpenEmblem);

	OpenUpgrade();
	
	m_LevelStatPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_ClassPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_EmblemPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetUpgradeMenu::OnOpen()
{
	Super::OnOpen();

	if(UMyGameInstance::Get->GetPlayerPawn())
	{
		UMyGameInstance::Get->GetPlayerPawn()->SetCameraTop();
	}
}

void UWidgetUpgradeMenu::OnClose()
{
	Super::OnClose();

	if(UMyGameInstance::Get->GetPlayerPawn())
	{
		UMyGameInstance::Get->GetPlayerPawn()->SetCameraCenter();
	}
}

void UWidgetUpgradeMenu::OpenUpgrade()
{
	m_LevelStatPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_ClassPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_EmblemPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	m_UpgradePanel->Open();
}

void UWidgetUpgradeMenu::OpenLevelStat()
{
	m_UpgradePanel->SetVisibility(ESlateVisibility::Collapsed);
	m_ClassPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_EmblemPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	m_LevelStatPanel->Open();
}

void UWidgetUpgradeMenu::OpenClass()
{
	m_UpgradePanel->SetVisibility(ESlateVisibility::Collapsed);
	m_LevelStatPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_EmblemPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ClassPanel->Open();
}

void UWidgetUpgradeMenu::OpenEmblem()
{
	m_UpgradePanel->SetVisibility(ESlateVisibility::Collapsed);
	m_LevelStatPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_ClassPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	m_EmblemPanel->Open();
}

