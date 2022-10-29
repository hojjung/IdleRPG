#include "Widgets/GameLevel/MainMenu/UpgradeMenu/WidgetUpgradeMenu.h"

#include "Manager/MyGameInstance.h"
#include "Widgets/Lib/WidgetLib.h"

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

	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnUpgrade);
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

	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnUpgrade);
}

void UWidgetUpgradeMenu::OpenLevelStat()
{
	m_UpgradePanel->SetVisibility(ESlateVisibility::Collapsed);
	m_ClassPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_EmblemPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	m_LevelStatPanel->Open();

	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnLevelStat);
}

void UWidgetUpgradeMenu::OpenClass()
{
	m_UpgradePanel->SetVisibility(ESlateVisibility::Collapsed);
	m_LevelStatPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_EmblemPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	m_ClassPanel->Open();

	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnClass);
}

void UWidgetUpgradeMenu::OpenEmblem()
{
	m_UpgradePanel->SetVisibility(ESlateVisibility::Collapsed);
	m_LevelStatPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_ClassPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	m_EmblemPanel->Open();

	UWidgetLib::SetCurrentButton(m_CurrentBtn, m_BtnEmblem);
}

