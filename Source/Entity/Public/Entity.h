#pragma once

#include "Modules/ModuleInterface.h"
#include "CoreMinimal.h"
#include "EngineMinimal.h"

#define PRINTFSCREEN(Format, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.5, FColor::Green, *FString::Printf(TEXT(Format), ##__VA_ARGS__),true)
#define PRINTF(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT(Format), ##__VA_ARGS__)) PRINTFSCREEN(Format, ##__VA_ARGS__)
#define PRINT3DF(Location,Format,...) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *FString::Printf(TEXT(Format), ##__VA_ARGS__), nullptr,  FColor::Orange, 0, false);
#define PRINT3D(Location,fString) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *fString, nullptr,  FColor::Orange, 0, false);
#define PRINT3DGreen(Location,fString) if (GEngine) DrawDebugString(GEngine->GetWorldFromContextObject(this,EGetWorldErrorMode::LogAndReturnNull), Location, *fString, nullptr,  FColor::Green, 0, false);


class  FEntityModule: public IModuleInterface
{
	
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};

