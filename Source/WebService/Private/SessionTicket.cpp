// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionTicket.h"

#include "PlayFabAuthenticationContext.h"
#include "PlayFabSettings.h"
#include "Kismet/GameplayStatics.h"
#include "WebService.h"

TSharedPtr<UPlayFabAuthenticationContext> USessionTicket::CreateAuthCon(const FString* newSessonTicket)
{
	if (newSessonTicket) //로그인후 새로 만드는거임
		{
		
		PlayFab::PlayFabSettings::SetClientSessionTicket(*newSessonTicket);
		FString SessionTicket = PlayFab::PlayFabSettings::GetClientSessionTicket();

		USessionTicket* SaveGameInstance = Cast<USessionTicket>(UGameplayStatics::CreateSaveGameObject(USessionTicket::StaticClass()));
		SaveGameInstance->m_SessionTicket = SessionTicket;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance,TEXT("SessionTicket"), 0);
		
		return TSharedPtr<UPlayFabAuthenticationContext>(NewObject<UPlayFabAuthenticationContext>(), DeleterNot());
		}
	else //로그인 하기전 로컬 로드
		{
		USessionTicket* LoadedSession = Cast<USessionTicket>(UGameplayStatics::LoadGameFromSlot(TEXT("SessionTicket"), 0));

		if (LoadedSession && !LoadedSession->m_SessionTicket.IsEmpty()) //로드 성공
			{
			PlayFab::PlayFabSettings::SetClientSessionTicket(LoadedSession->m_SessionTicket);
			FString SessionTicket = PlayFab::PlayFabSettings::GetClientSessionTicket();

			return TSharedPtr<UPlayFabAuthenticationContext>(NewObject<UPlayFabAuthenticationContext>(), DeleterNot());
			}
		}
	return nullptr;
}
