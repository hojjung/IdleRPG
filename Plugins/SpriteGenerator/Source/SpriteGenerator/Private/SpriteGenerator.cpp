// Copyright 2020 JWL, Inc. All Rights Reserved.

#include "SpriteGenerator.h"

#define LOCTEXT_NAMESPACE "FSpriteGeneratorModule"

void FSpriteGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FSpriteGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSpriteGeneratorModule, SpriteGenerator)