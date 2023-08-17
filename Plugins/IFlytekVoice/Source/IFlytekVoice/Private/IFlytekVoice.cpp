// Copyright Epic Games, Inc. All Rights Reserved.

#include "IFlytekVoice.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FIFlytekVoiceModule"

void FIFlytekVoiceModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("IFlytekVoice")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	
#elif PLATFORM_MAC
   
	
#endif // PLATFORM_WINDOWS
	
}

void FIFlytekVoiceModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FIFlytekVoiceModule, IFlytekVoice)
