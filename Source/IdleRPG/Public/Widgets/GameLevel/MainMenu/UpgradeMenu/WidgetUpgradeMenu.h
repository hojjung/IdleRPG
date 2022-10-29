#pragma once

#include "CoreMinimal.h"
#include "WidgetClassPanel.h"
#include "WidgetEmblemPanel.h"
#include "WidgetUpgradePanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Widgets/GameLevel/MainMenu/WidgetMenuBase.h"
#include "WidgetUpgradeMenu.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API UWidgetUpgradeMenu : public UWidgetMenuBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetUpgradePanel* m_UpgradePanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetUpgradePanel* m_LevelStatPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetClassPanel* m_ClassPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UWidgetEmblemPanel* m_EmblemPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnUpgrade;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnLevelStat;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnClass;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* m_BtnEmblem;

	TWeakObjectPtr<UButton> m_CurrentBtn;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void OnOpen() override;

	virtual void OnClose() override;
	
	UFUNCTION()
	void OpenUpgrade();
	UFUNCTION()
	void OpenLevelStat();
	UFUNCTION()
	void OpenClass();
	UFUNCTION()
	void OpenEmblem();
};


