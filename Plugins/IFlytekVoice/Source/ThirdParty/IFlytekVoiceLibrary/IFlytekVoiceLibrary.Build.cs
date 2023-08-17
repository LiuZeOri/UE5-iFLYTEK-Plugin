// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class IFlytekVoiceLibrary : ModuleRules
{
	public IFlytekVoiceLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library

			// Delay-load the DLL, so we can load it from the right place first

			// Ensure that the DLL is staged along with the executable
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
           
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			
		}
	}
}
