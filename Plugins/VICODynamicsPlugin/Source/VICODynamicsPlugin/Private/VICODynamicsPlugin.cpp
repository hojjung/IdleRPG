// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#include "IVICODynamicsPlugin.h"
#include "VICODynamicsPluginPrivatePCH.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

#if PLATFORM_XBOXONE
#include "XboxOneAllowPlatformTypes.h"
#include <winbase.h>
#include "XboxOneHidePlatformTypes.h"
#elif PLATFORM_SWITCH
// Not currently used since UE4 build system does not support Switch dynamic libraries yet...
#include <stdlib.h>
#include <nn/fs.h>
#include <nn/nn_Assert.h>
#include <nn/os.h>
#include <nn/ro.h>
size_t nnReadAll(void* pOut, size_t bufferSize, const char* path)
{
	nn::Result result;
	nn::fs::FileHandle file;
	result = nn::fs::OpenFile(&file, path, nn::fs::OpenMode_Read);
	NN_ASSERT(result.IsSuccess());

	int64_t fileSize;
	result = nn::fs::GetFileSize(&fileSize, file);
	NN_ASSERT(result.IsSuccess());
	NN_ASSERT_LESS(fileSize, static_cast<int64_t>(bufferSize));

	size_t readSize;
	result = nn::fs::ReadFile(&readSize, file, 0, pOut, bufferSize);
	NN_ASSERT(result.IsSuccess());
	NN_ASSERT_EQUAL(static_cast<int64_t>(readSize), fileSize);

	nn::fs::CloseFile(file);

	return readSize;
}
#endif

#include "VICODynamicsSettings.h"

#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

#define LOCTEXT_NAMESPACE "FVICODynamicsPlugin"

class FVICODynamicsPlugin : public IVICODynamicsPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;	
	virtual void ShutdownModule() override;

	void InitVICODynamics();
	bool LoadVICODynamicsDLL(FString PluginsDir);
};

IMPLEMENT_MODULE(FVICODynamicsPlugin, VICODynamicsPlugin)
DEFINE_LOG_CATEGORY(LogVICODynamics)

#if PLATFORM_WINDOWS || PLATFORM_PS4 || PLATFORM_XBOXONE// || PLATFORM_SWITCH
static void* VICODynamicsDllHandle;
#endif
#if PLATFORM_SWITCH
char* cacheBuffer;
nn::ro::Module VDNModule;
nn::ro::RegistrationInfo VDNInfo;
#endif

#if PLATFORM_DESKTOP
#ifdef _WIN32
//  Windows
#define cpuid(info, x)    __cpuidex(info, x, 0)
#else
//  GCC Intrinsics
#include <cpuid.h>
void cpuid(int info[4], int InfoType) 
{
	__cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
}
#endif
#endif

void FVICODynamicsPlugin::StartupModule()
{
#if PLATFORM_DESKTOP
	int32 cpuInfo[4];
	cpuid(cpuInfo, 1);
	const bool sse3Supported = cpuInfo[2] & (1 << 0) || false;
	const bool sse4Supported = cpuInfo[2] & (1 << 19) || false;
	const bool fmaSupported = cpuInfo[2] & (1 << 12) || false;
	if (/*!sse4Supported || !fmaSupported*/!sse3Supported)
	{
		if (!sse3Supported)
		{
			UE_LOG(LogVICODynamics, Warning, TEXT("VICO Dynamics Plugin: SSE3 not supported on CPU"));
		}
		if (!sse4Supported)
		{
			UE_LOG(LogVICODynamics, Warning, TEXT("VICO Dynamics Plugin: SSE4 not supported on CPU"));
		}
		if (!fmaSupported)
		{
			UE_LOG(LogVICODynamics, Warning, TEXT("VICO Dynamics Plugin: FMA3 not supported on CPU"));
		}
		return;
	}
#endif

	UE_LOG(LogVICODynamics, Display, TEXT("Loading VICO Dynamics Plugin."));

	InitVICODynamics();
}

void FVICODynamicsPlugin::InitVICODynamics()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	VDSimulation = nullptr;

#if PLATFORM_WINDOWS || PLATFORM_PS4// || PLATFORM_XBOXONE
	VICODynamicsDllHandle = nullptr;

	// Try the Engine/Plugins directory first
	if (!LoadVICODynamicsDLL(FPaths::EnginePluginsDir()))
		LoadVICODynamicsDLL(FPaths::ProjectPluginsDir());

	// Continue with initialization if dll loads successfully
	if (VICODynamicsDllHandle)
	{
		VDSimulation = NewObject<UVDSimulation>();
		VDSimulation->AddToRoot();
}
	else
	{
		UE_LOG(LogVICODynamics, Error, TEXT("Could not load VICODynamics.dll. VICO Dynamics plugin is inactive."));
	}
#elif PLATFORM_MAC || PLATFORM_SWITCH || PLATFORM_XBOXONE
	VDSimulation = NewObject<UVDSimulation>();
	VDSimulation->AddToRoot();
#endif

	if (VDSimulation)
	{
		Released = false;
		UE_LOG(LogVICODynamics, Display, TEXT("VICO Dynamics plugin is loaded."));

#if WITH_EDITOR
		// register settings
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "VICODynamics",
				LOCTEXT("VICODynamicsSettingsName", "VICO Dynamics"),
				LOCTEXT("VICODynamicsSettingsDescription", "Configure the VICO Dynamics plug-in."),
				GetMutableDefault<UVICODynamicsSettings>()
			);

			if (SettingsSection.IsValid())
			{
				//SettingsSection->OnModified().BindRaw(this, &FVICODynamicsPlugin::HandleSettingsSaved);
			}
		}
#endif
	}
}

void FVICODynamicsPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (VDSimulation)
	{
#if WITH_EDITOR
		// unregister settings
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "VICODynamics");
		}
#endif

		if (static_cast<int32>(VDSimulation->GetUniqueID()) >= 0) // Only try to remove if with a valid Index
		{
			VDSimulation->RemoveFromRoot();
			VDSimulation->MarkPendingKill();
		}

		VDSimulation = nullptr;
		Released = true;

#if PLATFORM_WINDOWS || PLATFORM_XBOXONE || PLATFORM_PS4
		if (VICODynamicsDllHandle)
			FPlatformProcess::FreeDllHandle(VICODynamicsDllHandle);
#elif PLATFORM_SWITCH
// 		if (VICODynamicsDllHandle)
// 		{
// 			nn::ro::UnloadModule(&VDNModule);
// 			nn::ro::UnregisterModuleInfo(&VDNInfo);
// 			nn::ro::Finalize();
// 
// 			delete[] cacheBuffer;
// 			cacheBuffer = nullptr;
// 		}
#endif // PLATFORM_WINDOWS || PLATFORM_XBOXONE || PLATFORM_PS4

	}
}

#if PLATFORM_WINDOWS || PLATFORM_PS4 || PLATFORM_XBOXONE// || PLATFORM_SWITCH
bool FVICODynamicsPlugin::LoadVICODynamicsDLL(FString PluginsDir)
{
#if PLATFORM_WINDOWS// || PLATFORM_XBOXONE
	FString PlatformStr;
#if PLATFORM_WINDOWS
	PlatformStr = (PLATFORM_64BITS ? TEXT("Win64/") : TEXT("Win32/"));
 #elif PLATFORM_XBOXONE
 	PlatformStr = TEXT("XboxOne/");
#endif
		
	FString VDynamicsDllDir = PluginsDir / TEXT("VICODynamicsPlugin/Source/VICODynamicsPlugin/VICODynamics/Lib/") / PlatformStr;

	bool FailedToFindPluginDirectory = false;

	// Check Non-Marketplace folder first
	FailedToFindPluginDirectory = !FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*VDynamicsDllDir.ToLower());
	if (FailedToFindPluginDirectory)
	{
		// Check Marketplace folder
		VDynamicsDllDir = PluginsDir / TEXT("Marketplace/") / TEXT("VICODynamicsPlugin/Source/VICODynamicsPlugin/VICODynamics/Lib/") / PlatformStr;
		FailedToFindPluginDirectory = !FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*VDynamicsDllDir.ToLower());
	}

	if (FailedToFindPluginDirectory)
	{
		VICODynamicsDllHandle = nullptr;

		UE_LOG(LogVICODynamics, Display, TEXT("VICODynamics DLL Folder not found: \"%s\""), *VDynamicsDllDir);
		return false;
	}
	else
	{
		UE_LOG(LogVICODynamics, Display, TEXT("VICODynamics DLL Folder found: \"%s\"!"), *VDynamicsDllDir);
	}

	FString DllPath;
	FString Config;
	FString ParallelSuffix;

#if VICODYNAMICS_USE_PARALLEL
	ParallelSuffix += TEXT("_Parallel");
#endif

#if UE_BUILD_DEBUG
#	if defined(VICODYNAMICS_DEBUG)
	Config = TEXT("Debug");
#	else
	Config = TEXT("Checked");
#	endif
#elif (UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Config = TEXT("Release");
#else
#	if defined(VICODYNAMICS_DEBUG)
	Config = TEXT("Debug");
#	else
	Config = TEXT("Profile");
#	endif
#endif

	DllPath = Config + ParallelSuffix + TEXT("/");
	
	FString DLLDirPushed = VDynamicsDllDir + DllPath;
	FPlatformProcess::PushDllDirectory(*DLLDirPushed);

	FString DllFileName = TEXT("VICODynamics_") + Config + ParallelSuffix;

#if PLATFORM_WINDOWS || PLATFORM_XBOXONE
	DllFileName += TEXT(".dll");
	VICODynamicsDllHandle = FPlatformProcess::GetDllHandle(*DllFileName);
#endif

	FPlatformProcess::PopDllDirectory(*DLLDirPushed);
#elif PLATFORM_PS4
	FString DllPath;
	FString Config;
	FString ParallelSuffix;

#if VICODYNAMICS_USE_PARALLEL
	ParallelSuffix += TEXT("_Parallel");
#endif

#if UE_BUILD_DEBUG
#	if _DEBUG
	Config = TEXT("Debug");
#	else
	Config = TEXT("Profile");
#	endif
#elif (UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Config = TEXT("Release");
#else
	Config = TEXT("Profile");
#endif

	DllPath = Config + ParallelSuffix + TEXT("/");

	FString DLLDirPushed = FPaths::GameDir() / TEXT("prx");
	FPlatformProcess::PushDllDirectory(*DLLDirPushed);

	FString DllFileName = TEXT("libVICODynamics_") + Config + ParallelSuffix + TEXT(".prx");
	VICODynamicsDllHandle = FPlatformProcess::GetDllHandle(*DllFileName);

	FPlatformProcess::PopDllDirectory(*DLLDirPushed);
#elif PLATFORM_SWITCH // Not currently used since UE4 build system does not support Switch dynamic libraries yet...
	FString DllFolder;
	FString Config;
	FString ParallelSuffix;

	#if VICODYNAMICS_USE_PARALLEL
	ParallelSuffix += TEXT("_Parallel");
	#endif

	#if UE_BUILD_DEBUG
	#	if _DEBUG
	Config = TEXT("Debug");
	#	else
	Config = TEXT("Profile");
	#	endif
	#elif (UE_BUILD_SHIPPING || UE_BUILD_TEST)
	Config = TEXT("Release");
	#else
	Config = TEXT("Profile");
	#endif

	DllFolder = Config + ParallelSuffix + TEXT("/");
	FString DLLDir = FPaths::ProjectDir() / TEXT(".nrr");
	FString DllFilename = TEXT("VICODynamics_") + Config + ParallelSuffix;
	
	nn::Result result;
	static const size_t MaxFileSize = 0x400000;
	void* nro;
	void* bss;
	void* nrr;
	size_t imageSize;
	size_t bufferSize;
	size_t nrrSize;

	nn::ro::Initialize();

	// Get the buffer size needed for the file system metadata cache.
	size_t cacheSize = 0;
	result = nn::fs::QueryMountRomCacheSize(&cacheSize);
	NN_ASSERT(result.IsSuccess());

	// Allocate the cache buffer.
	cacheBuffer = new(std::nothrow) char[cacheSize];
	NN_ASSERT_NOT_NULL(cacheBuffer);

	// Mount the file system.
	result = nn::fs::MountRom("rom", cacheBuffer, cacheSize);
	NN_ABORT_UNLESS_RESULT_SUCCESS(result);

	// Load the NRR file.
	nrr = FPlatformMemory::BaseAllocator()->Malloc(MaxFileSize, nn::os::MemoryPageSize);
	auto NrrLoadPath = *FString::Printf(TEXT("%s/%s.nrr"), *(DLLDir / DllFolder), *DllFilename);
	nrrSize = nnReadAll(nrr, MaxFileSize, TCHAR_TO_UTF8(NrrLoadPath));

	// Register the NRR file.
	result = nn::ro::RegisterModuleInfo(&VDNInfo, nrr);
	NN_ASSERT(result.IsSuccess());

	nro = FPlatformMemory::BaseAllocator()->Malloc(MaxFileSize, nn::os::MemoryPageSize);
	auto NroLoadPath = *FString::Printf(TEXT("%s/%s.nro"), *(DLLDir / DllFolder), *DllFilename);
	imageSize = nnReadAll(nro, MaxFileSize, TCHAR_TO_UTF8(NroLoadPath));

	result = nn::ro::GetBufferSize(&bufferSize, nro);
	NN_ASSERT(result.IsSuccess());
	if (bufferSize != 0)
	{
		bss = FPlatformMemory::BaseAllocator()->Malloc(bufferSize, nn::os::MemoryPageSize);
	}
	else
	{
		bss = 0;
	}

	// Load the NRO file (delayed resolution for the symbols).
	result = nn::ro::LoadModule(&VDNModule, nro, bss, bufferSize, nn::ro::BindFlag_Now);
	NN_ASSERT(result.IsSuccess());

	if (result.IsSuccess())
		VICODynamicsDllHandle = nro;
#endif

	return VICODynamicsDllHandle != nullptr;
}
#endif

#undef LOCTEXT_NAMESPACE