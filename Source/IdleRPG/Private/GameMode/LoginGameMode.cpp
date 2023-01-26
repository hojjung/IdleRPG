#include "GameMode/LoginGameMode.h"


ALoginGameMode::ALoginGameMode()
{
	static ConstructorHelpers::FClassFinder<UWidgetLoginCanvas> FoundWW(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/Login/WB_LoginCanvas.WB_LoginCanvas_C'"));
	
	m_ClassWidget = FoundWW.Class;
}

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_Canvas = CreateWidget<UWidgetLoginCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();

		m_Canvas->StartPlayfabLogin();
	}
}

void ALoginGameMode::PrintInfoText(FText str, FLinearColor color)
{
	m_Canvas->PrintInfoText(str, color);
}

void ALoginGameMode::SetConfirmPanel(const FText& txt, UWidgetConfirmPanel::FOnClick onCancel, UWidgetConfirmPanel::FOnClick onConfirm)
{
	m_Canvas->GetConfirmPanel()->SetConfirmPanel(txt,onCancel,onConfirm);
}
