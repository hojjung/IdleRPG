//////////////////////////////////////////////////////
// Copyright (C) Microsoft. 2018. All rights reserved.
//////////////////////////////////////////////////////


#pragma once

#include "CoreMinimal.h"
#include "PlayFabCommon.h"
#include "PlayFabCommonUtils.h"
#include "PlayFabError.h"

namespace PlayFab
{
    class PlayFabSettings
    {
    public:
        static FString sdkVersion;
        static FString buildIdentifier;
        static FString versionString;
        static FString connectionString;

        // Getters
        static FString GetClientSessionTicket()
        {
            return IPlayFabCommonModuleInterface::Get().GetClientSessionTicket();
        }
        static FString GetEntityToken()
        {
            return IPlayFabCommonModuleInterface::Get().GetEntityToken();
        }
        static FString GeneratePfUrl(const FString& urlPath)
        {
            return IPlayFabCommonModuleInterface::Get().GeneratePfUrl(urlPath);
        }
        static FString GetLocalApiServer()
        {
            return PlayFabCommon::PlayFabCommonUtils::GetLocalSettingsFileProperty(TEXT("LocalApiServer"));
        }
        static FString GetConnectionString()
        {
            return IPlayFabCommonModuleInterface::Get().GetConnectionString();
        }

        // Setters
        static void SetClientSessionTicket(const FString&  clientSessionTicket)
        {
            IPlayFabCommonModuleInterface::Get().SetClientSessionTicket(clientSessionTicket);
        }
        static void SetEntityToken(const FString&  entityToken)
        {
            IPlayFabCommonModuleInterface::Get().SetEntityToken(entityToken);
        }
        static void SetConnectionString(const FString& connectionStr)
        {
            IPlayFabCommonModuleInterface::Get().SetConnectionString(connectionStr);
        }
    };
}
