
#include "WidgetContract.h"

#include "BUITween.h"
#include "BUITween.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/PlayfabManager.h"


void UWidgetContract::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_TextInputNickname->OnTextChanged.AddDynamic(this,&UWidgetContract::OnTextChanged);

	m_BtnConfirm->OnClicked.AddDynamic(this, &UWidgetContract::OnConfirm);

	m_URL_Usage = TEXT("https://sites.google.com/d/1HlA8Ld1kt9gpu2iDYwFzINQLOfAuleM7/p/1THdXylNcaX2yBQFXU8PdHxzcTIrW8lOn/edit");

	m_URL_Private = TEXT("https://sites.google.com/d/1eNgJYa-FX0S-TKT_1oi3EwmuFPsj_Taf/p/1BQMc0eId0ROySCOxCIdI18dfaCqWvnh_/edit");

	m_URL_Refund = TEXT("https://sites.google.com/d/1v7y1DvwBQM8vmNTKL2WO-Z4-NfBZJ1hK/p/1gcJeDXzLEGjHh2KXwu7hpnNISTkGvR6o/edit");
	//Test

	//UMyGameInstance::Get->StartGame();
}

void UWidgetContract::OnConfirm()
{
	if(!m_CheckUsage->IsChecked())
	{
		m_OnTextAlert.Execute(NSLOCTEXT("UWidgetContract","GameUsage Check","게임 이용약관 미수락"), FLinearColor::Red);
		return;
	}
	if(!m_CheckPrivate->IsChecked())
	{
		m_OnTextAlert.Execute(NSLOCTEXT("UWidgetContract","GamePrivate Check","게임내 개인정보 수집 미수락"), FLinearColor::Red);
		return;
	}
	if(!m_CheckRefund->IsChecked())
	{
		m_OnTextAlert.Execute(NSLOCTEXT("UWidgetContract","GameRefund Check","청약철회 정책 미수락"), FLinearColor::Red);
		return;
	}
	FString CrntStr =  m_TextInputNickname->GetText().ToString();

	if(CrntStr.IsEmpty())
	{
		m_OnTextAlert.Execute(NSLOCTEXT("UWidgetContract","EmptyChar","비어있는 아이디"), FLinearColor::Red);
		return;
	}

	// if(UMyGameInstance::Get->m_BadwordTable->NicknameHasNonChar(CrntStr))
	// {
	// 	m_OnTextAlert.Execute(NSLOCTEXT("UWidgetContract","NonChar","지원되지 않는 문자"), FLinearColor::Red);
	// 	return;
	// }
	//
	// if(UMyGameInstance::Get->m_BadwordTable->NicknameHasBadWord(CrntStr))
	// {
	// 	m_OnTextAlert.Execute(NSLOCTEXT("UWidgetContract","Badword","허용되지 않은 비속어"), FLinearColor::Red);
	// 	return;
	// }
	
	//동의 다되었는지 체크
	//나쁜 닉네임인지 체크
	//닉네임 중복되는지 체크
	const FWebServiceModule& WebModule = FModuleManager::LoadModuleChecked< FWebServiceModule >("WebService");
	
	WebModule.m_PlayfabManager->RequestSetNickname(CrntStr);
	
	SetVisibility(ESlateVisibility::Collapsed);
	//UMyGameInstance::Get->StartGame();
}

FReply UWidgetContract::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	CheckTextTouch(InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetContract::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	CheckTextTouch(InMouseEvent);

	return FReply::Handled();
}

void UWidgetContract::CheckTextTouch(const FPointerEvent& eve)
{
	FVector2D MousePos = eve.GetScreenSpacePosition();

	if(m_TextUsage->GetCachedGeometry().GetRenderBoundingRect().ContainsPoint(MousePos))
	{
		OpenWebUsage();
	}
	else if(m_TextPrivate->GetCachedGeometry().GetRenderBoundingRect().ContainsPoint(MousePos))
	{
		OpenWebPrivate();
	}
	else if(m_TextRefund->GetCachedGeometry().GetRenderBoundingRect().ContainsPoint(MousePos))
	{
		OpenWebRefund();
	}
}

void UWidgetContract::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	UBUITween::Create( this, 0.25f )
	.FromScale(FVector2D(0,0))
	.ToScale(FVector2D(1.0f,1.0f))
	.Begin();
}

void UWidgetContract::OnTextChanged(const FText& text)
{
	FString Str = text.ToString();
	
	int Diff = Str.Len() - 10;
	
	if(Diff > 0)
	{
		Str = Str.LeftChop(Diff);
	}

	m_TextInputNickname->SetText(FText::FromString(Str));
}

void UWidgetContract::OpenWebUsage()
{
	UKismetSystemLibrary::LaunchURL(m_URL_Usage);
}

void UWidgetContract::OpenWebPrivate()
{
	UKismetSystemLibrary::LaunchURL(m_URL_Private);
}

void UWidgetContract::OpenWebRefund()
{
	UKismetSystemLibrary::LaunchURL(m_URL_Refund);
}
