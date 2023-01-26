#include "Widgets/WidgetLoginCanvas.h"

#include "BUITween.h"
#include "WidgetContract.h"
#include "Components/CanvasPanel.h"
#include "Manager/MyGameInstance.h"
#include "Manager/PlayfabManager.h"


void UWidgetLoginCanvas::PrintInfoText(FText str, FLinearColor color)
{
	m_AlertInfoWindow->ShowText(str, color);
}

UWidgetConfirmPanel* UWidgetLoginCanvas::GetConfirmPanel()
{
	return m_ConfirmPanel;
}

void UWidgetLoginCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_WebService = &FModuleManager::LoadModuleChecked< FWebServiceModule >("WebService");

	OnCloseNews();
}

void UWidgetLoginCanvas::OnCloseNews()
{
	m_NewsCanvas->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetLoginCanvas::StartPlayfabLogin()
{
	m_fTimer = 0.f;
	
	m_ImgBlink->SetVisibility(ESlateVisibility::Collapsed);
	
	m_WidgetContract->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnCloseNews->OnClicked.AddDynamic(this, &UWidgetLoginCanvas::OnCloseNews);

	m_WebService->m_PlayfabManager->RequestTitleNews(FNewsDele::CreateUObject(this, &UWidgetLoginCanvas::OnSuccessGetTitleNews));

	m_WebService->m_PlayfabManager->RequestServerOpenCheck();
	
	m_WebService->m_PlayfabManager->m_OnLoginEnd.BindUObject(this, &UWidgetLoginCanvas::OpenConfirmPanel);

	m_WebService->m_PlayfabManager->m_OnTextAlert.BindUObject(this, &UWidgetLoginCanvas::PrintInfoText);

	m_WebService->m_PlayfabManager->StartPlayfabLogin();
	
	m_WidgetContract->m_OnTextAlert.BindUObject(this, &UWidgetLoginCanvas::PrintInfoText);
	
	
}

void UWidgetLoginCanvas::OnSuccessGetTitleNews(const PlayFab::ClientModels::FGetTitleNewsResult& rslt)
{
	m_NewsCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	UBUITween::Create( m_NewsCanvas, 0.25f )
	.FromScale(FVector2D(0,0))
	.ToScale(FVector2D(1.0f,1.0f))
	.Begin();
	
	TArray<PlayFab::ClientModels::FTitleNewsItem> NewsArray = rslt.News;

	FTimespan KoreanTime(9,0,0);
	
	FDateTime Time = NewsArray[0].Timestamp + KoreanTime;

	FString Title = FString::Printf(TEXT("%s-%s"), *NewsArray[0].Title, *Time.ToString());
	
	m_TextTitle->SetText(FText::FromString(Title));
	
	m_TextTitleNews->SetText(FText::FromString(NewsArray[0].Title));

	m_TextTitleNews->SetText(FText::FromString(NewsArray[0].Body));
}

void UWidgetLoginCanvas::OpenConfirmPanel()
{
	m_WidgetContract->Show();
}

FReply UWidgetLoginCanvas::TryStart()
{
	const FString& Nick = m_WebService->m_PlayfabManager->GetNickName();
	
	if(Nick.IsEmpty() || m_NewsCanvas->IsVisible())
	{
		return FReply::Handled(); 
	}
	
	UMyGameInstance::Get->StartGame();

	return FReply::Handled();
}

FReply UWidgetLoginCanvas::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	return TryStart(); 
}

FReply UWidgetLoginCanvas::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return TryStart();
}

void UWidgetLoginCanvas::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FString& Nick = m_WebService->m_PlayfabManager->GetNickName();

	if(Nick.IsEmpty())
	{
		return ; 
	}

	m_fTimer += InDeltaTime;

	if (m_fTimer >=0.15f)
	{
		if(m_ImgBlink->IsVisible())
		{
			m_ImgBlink->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			m_ImgBlink->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		m_fTimer = 0;
	}
}
