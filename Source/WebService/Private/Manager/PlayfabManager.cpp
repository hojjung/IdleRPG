#include "Manager/PlayfabManager.h"
#include "GoogleLogin.h"
#include "OnlineSubsystem.h"
#include "PlayFabUtilities.h"
#include "SessionTicket.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Serialization/JsonSerializer.h"

#define LOCTEXT_NAMESPACE "PlayfabManager"

void PlayfabManager::OnErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	FString CodeString = UPlayFabUtilities::getErrorText(ErrorResult.ErrorCode);
	
	if(ErrorResult.ErrorCode == PlayFab::PlayFabErrorCode::PlayFabErrorProfaneDisplayName
		|| ErrorResult.ErrorCode == PlayFab::PlayFabErrorCode::PlayFabErrorInvalidDisplayNameRandomSuffixLength
		|| ErrorResult.ErrorCode == PlayFab::PlayFabErrorCode::PlayFabErrorAllowNonUniquePlayerDisplayNamesDisableNotAllowed)
	{
		m_OnLoginEnd.ExecuteIfBound();
	}
}

PlayfabManager::PlayfabManager()
{
	m_CurrentVersionName = TEXT("1");

	m_bIsServerClosed = false;

	GetClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	m_Auth = USessionTicket::CreateAuthCon();

	m_bIsLoginSuccess = false;
}

void PlayfabManager::StartPlayfabLogin()
{
	if(m_bIsLoginSuccess)
	{
		return;
	}
#if PLATFORM_WINDOWS
	m_OnTextAlert.Execute(LOCTEXT("Try Login With Desktop", "로그인 시도-PC"), FLinearColor::White);

	PlayFab::ClientModels::FLoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = TEXT("JungPC TestID3");
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	
	request.AuthenticationContext =	m_Auth;
	
	GetClientAPI->LoginWithCustomID(request,FLoginDele::CreateRaw(this, &PlayfabManager::OnSuccessPlayfabLogin),
		FFailDele::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));

#endif

#if PLATFORM_ANDROID
	m_OnTextAlert.Execute(LOCTEXT("Try Login With Session TIcket","로그인 시도-Google"),  FLinearColor::White);
	
	PlayFab::ClientModels::FLoginWithGoogleAccountRequest SessionRequest;
	
	SessionRequest.CreateAccount = false;
	
	SessionRequest.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;
	
	SessionRequest.AuthenticationContext = m_Auth;

	GetClientAPI->LoginWithGoogleAccount(SessionRequest,
													PlayFab::UPlayFabClientAPI::FLoginWithGoogleAccountDelegate::CreateRaw(
														this, &PlayfabManager::OnSuccessPlayfabLogin),
													PlayFab::FPlayFabErrorDelegate::CreateRaw(
														this, &PlayfabManager::OnSessionLoginErrorPlayfabReq)
	);
#endif
}

void PlayfabManager::HandleExternalUIClose(TSharedPtr<const FUniqueNetId> uniqueId, const int ControllerIndex, const FOnlineError& error)
{
	if (error.bSucceeded)
	{
		m_OnTextAlert.Execute(LOCTEXT("SUCCESS-GoogleLogin", "구글 로그인 성공"), FLinearColor::Green);
		m_OnTextAlert.Execute(error.GetErrorMessage(), FLinearColor::Yellow);
		
		TryLoginPlayfabGoogle();
	}
	else
	{
		m_OnTextAlert.Execute(LOCTEXT("FAIL-GoogleLoginFail-2", "실패-앱을 종료후 구글 계정 로그인 먼저해주세요"), FLinearColor::Red);
	}
}

void PlayfabManager::TryLoginPlayfabGoogle()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	IOnlineIdentityPtr OnlineIdentity = Subsystem->GetIdentityInterface();

	PlayFab::ClientModels::FLoginWithGoogleAccountRequest request;
	request.CreateAccount = true;
	request.ServerAuthCode = OnlineIdentity->GetAuthToken(0);
	request.TitleId = GetDefault<UPlayFabRuntimeSettings>()->TitleId;

	GetClientAPI->LoginWithGoogleAccount(request,FLoginDele::CreateRaw(this, &PlayfabManager::OnSuccessPlayfabLogin),
													   FFailDele::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));
}

void PlayfabManager::OnSuccessPlayfabLogin(const PlayFab::ClientModels::FLoginResult& Result)
{
	m_OnTextAlert.Execute(LOCTEXT("SUCCESS-Playfab Login Success", "플레이팹 로그인 성공"), FLinearColor::Green);

	FString SeTicket = Result.SessionTicket;

	m_Auth = USessionTicket::CreateAuthCon(&SeTicket);

	m_PlayfabID = Result.PlayFabId;

	m_LastLoginTime = Result.LastLoginTime;
	
	FTimespan KoreanTime(9,0,0);
	
	m_LastLoginTime += KoreanTime;
	
	RequestGetAccountInfo();
}

void PlayfabManager::OnSessionLoginErrorPlayfabReq(const FFailRslt& ErrorResult)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (!Subsystem)
	{
		return;
	}
	
	
	
	m_OnTextAlert.Execute(LOCTEXT("Try Login With Android","로그인 세션 만료,구글로그인 시도"), FLinearColor::White);
	
	IOnlineExternalUIPtr ExternalUi = Subsystem->GetExternalUIInterface();

	if (!ExternalUi)
	{
		m_OnTextAlert.Execute(LOCTEXT("FAIL-GoogleLoginFail-1","FAIL-GoogleLoginFail-1"), FLinearColor::White);
		return;
	}

	ExternalUi->ShowLoginUI(0, false, false, FOnLoginUIClosedDelegate::CreateRaw(this, &PlayfabManager::HandleExternalUIClose));
}

void PlayfabManager::RequestSetNickname(FString str)
{
	m_OnTextAlert.Execute(LOCTEXT("Request Nickname", "닉네임 요청 전송"), FLinearColor::White);
	
	PlayFab::ClientModels::FUpdateUserTitleDisplayNameRequest DisplayReq;
	DisplayReq.DisplayName = str;
	
	GetClientAPI->UpdateUserTitleDisplayName(DisplayReq,FNicknameDele::CreateRaw(this, &PlayfabManager::OnNickNameSetSuccess),PlayFab::FPlayFabErrorDelegate::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));
}

bool PlayfabManager::IsLoginSuccess() const
{
	return  m_bIsLoginSuccess;
}

void PlayfabManager::OnNickNameSetSuccess(const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result)
{
	m_LoadedNickname = result.DisplayName;

	m_bIsLoginSuccess = true;
	//int InsertIndex = m_LoadedNickname.Len() - 4; 
	//m_LoadedNickname.InsertAt(InsertIndex, '#');
}

void PlayfabManager::RequestGetServerTime()
{
	PlayFab::ClientModels::FGetTimeRequest Req;
	 	GetClientAPI->GetTime(Req,
	 		PlayFab::UPlayFabClientAPI::FGetTimeDelegate::CreateRaw(this,&PlayfabManager::OnSuccessTimeGet),
	 		PlayFab::FPlayFabErrorDelegate::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));
}

void PlayfabManager::OnSuccessTimeGet(const PlayFab::ClientModels::FGetTimeResult& rslt)
{
	m_CurrentTime = rslt.Time;
	
	FTimespan KoreanTime(9,0,0);
	
	m_CurrentTime+=KoreanTime;
}


void PlayfabManager::RequestVersionCheck()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;

	Req.FunctionName = TEXT("CheckVersion");

	Req.GeneratePlayStreamEvent = true;

	GetClientAPI->ExecuteCloudScript(
		Req, FExeCScriptDele::CreateRaw(this, &PlayfabManager::OnVersionCheckCloudScriptSuccess),
		FFailDele::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));
}

void PlayfabManager::OnVersionCheckCloudScriptSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}

	m_ServerVersion = JsonObject->GetStringField(TEXT("ServerVersionName"));
	
	if(m_CurrentVersionName == m_ServerVersion)
	{
		
		StartPlayfabLogin();
	}
	else
	{
		m_OnTextAlert.Execute(LOCTEXT("Version Changed", "업데이트 해주세요!"), FLinearColor::Red);
	}
}


void PlayfabManager::RequestServerOpenCheck()
{
	PlayFab::ClientModels::FExecuteCloudScriptRequest Req;
	
	Req.FunctionName =TEXT( "CheckServerOpen");
	
	Req.GeneratePlayStreamEvent = true;
	
	GetClientAPI->ExecuteCloudScript(Req,FExeCScriptDele::CreateRaw(this, &PlayfabManager::OnServerCloseCheckScriptSuccess),
		FFailDele::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));
}

void PlayfabManager::OnServerCloseCheckScriptSuccess(const FExeCScriptRslt& rslt)
{
	FString CachedJsonString = rslt.FunctionResult.toJSONString();

	TSharedPtr<FJsonObject> JsonObject; TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(CachedJsonString);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}

	bool IsServerOpen = JsonObject->GetBoolField(TEXT("IsServerOpen"));
	
	m_bIsServerClosed = !IsServerOpen;

	m_ServerCloseOpenTime = JsonObject->GetStringField(TEXT("ServerCloseOpenTime"));

	if(!m_bIsServerClosed)
	{
		m_OnTextAlert.Execute(LOCTEXT("Server Open", "서버 사용 가능"), FLinearColor::Green);
		
		RequestVersionCheck();
	}
	else
	{
		FText ServerClosedTxt = LOCTEXT("Server Closed", "서버 사용 불가");

		FString Str = FString::Printf(TEXT("%s:%s"),*ServerClosedTxt.ToString(), *m_ServerCloseOpenTime); 

		m_OnTextAlert.Execute(FText::FromString(Str), FLinearColor::Red);
	}
}


void PlayfabManager::RequestGetAccountInfo()
{
	m_OnTextAlert.Execute(LOCTEXT("RequestGetAccountInfo", "계정 정보 요청"), FLinearColor::White);
	
	FGetAccntInfoReq Req;
	
	GetClientAPI->GetAccountInfo(Req, FGetAccntInfoDele::CreateRaw(this, &PlayfabManager::OnSuccessGetAccountInfo),
	                             FFailDele::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));
}

const FString& PlayfabManager::GetNickName()
{
	return m_LoadedNickname;
}

void PlayfabManager::OnSuccessGetAccountInfo(const FGetAccntInfoRslt& rslt)
{
	if (rslt.AccountInfo->TitleInfo->isBanned)
	{
		m_OnTextAlert.Execute(LOCTEXT("Banned Player", "정지된 플레이어"), FLinearColor::Red);
		return;
	}

	if (rslt.AccountInfo->TitleInfo->DisplayName.IsEmpty())
	{
		m_OnLoginEnd.ExecuteIfBound();
		return;
	}

	m_OnTextAlert.Execute(LOCTEXT("Welcome", "환영 합니다"), FLinearColor::White);

	PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult ResultNickname;
	
	ResultNickname.DisplayName = rslt.AccountInfo->TitleInfo->DisplayName;
	
	OnNickNameSetSuccess(ResultNickname);
}


void PlayfabManager::RequestTitleNews(FNewsDele onEnd)
{
	PlayFab::ClientModels::FGetTitleNewsRequest Req;
	Req.Count = 5;
	GetClientAPI->GetTitleNews(Req, onEnd, FFailDele::CreateRaw(this, &PlayfabManager::OnErrorPlayfabReq));
}

#undef LOCTEXT_NAMESPACE
