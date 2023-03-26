// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineError.h"
#include "PlayFab.h"
#include "WebService.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabClientAPI.h"
#include "Delegates/DelegateCombinations.h"
#include "Delegates/DelegateCombinations.h"

#include "Net/UnrealNetwork.h"
#include "Interfaces/OnlineStoreInterface.h"


typedef PlayFab::UPlayFabClientAPI::FGetTitleNewsDelegate FNewsDele;

typedef PlayFab::UPlayFabClientAPI::FUpdateUserTitleDisplayNameDelegate FNicknameDele;

typedef PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate FLoginDele;


typedef PlayFab::UPlayFabClientAPI::FUpdateUserDataDelegate FUpdateDele;
typedef PlayFab::ClientModels::FUpdateUserDataRequest FUpdateReq;
typedef PlayFab::ClientModels::FUpdateUserDataResult FUpdateRslt;
//
typedef PlayFab::UPlayFabClientAPI::FExecuteCloudScriptDelegate FExeCScriptDele;
typedef PlayFab::ClientModels::FExecuteCloudScriptRequest FExeCScriptReq;
typedef PlayFab::ClientModels::FExecuteCloudScriptResult FExeCScriptRslt;
//
typedef PlayFab::UPlayFabClientAPI::FGetUserDataDelegate FGetUsrDataDele;
typedef PlayFab::ClientModels::FGetUserDataRequest FGetUsrDataReq;
typedef PlayFab::ClientModels::FGetUserDataResult FGetUsrDataRslt;
//
typedef PlayFab::FPlayFabErrorDelegate FFailDele;
typedef PlayFab::FPlayFabCppError FFailRslt;
//
typedef PlayFab::UPlayFabClientAPI::FGetAccountInfoDelegate FGetAccntInfoDele;
typedef PlayFab::ClientModels::FGetAccountInfoRequest FGetAccntInfoReq;
typedef PlayFab::ClientModels::FGetAccountInfoResult FGetAccntInfoRslt;

class WEBSERVICE_API PlayfabManager
{
public:
	PlayfabManager();

	DECLARE_DELEGATE(FOnLoginEnd);


	FOnLoginEnd m_OnLoginEnd;

	FOnLoginEnd m_OnDataSet;
	
	FOnTextAlert m_OnTextAlert;

protected:
	bool m_bIsServerClosed;
	
	PlayFabClientPtr GetClientAPI = nullptr;
	
	FString m_SessionTicket;
	
	FString m_LoadedNickname;

	bool m_bIsLoginSuccess;

	TSharedPtr<UPlayFabAuthenticationContext> m_Auth;
	
	FString m_PlayfabID;
	
	FDateTime m_LastLoginTime;
	
	FDateTime m_LastLogoutTime;
	
	FDateTime m_CurrentTime;
	
	FString m_ServerCloseOpenTime;
	
	FString m_ServerVersion;
	
	FString m_CurrentVersionName;
	
protected:
	void OnErrorPlayfabReq(const FFailRslt& ErrorResult);
	
	void OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result);

	void RequestVersionCheck();
	
	void OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt);

	void OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt);

	void RequestGetServerTime();
	
	void OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& rslt);
	
	void OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt);

	void HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error);

	void OnSessionLoginErrorPlayfabReq(const FFailRslt& ErrorResult);

	void TryLoginPlayfabGoogle();
	
	void OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result);

	void RequestGetAccountInfo();
	
public:
	void StartPlayfabLogin();
	
	void RequestTitleNews(FNewsDele onEnd);
	
	void RequestServerOpenCheck();
	
	const FString& GetNickName();

	void RequestSetNickname(FString str);

	bool IsLoginSuccess() const;
};
