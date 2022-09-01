// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AsyncTaskFromUClass : ModuleRules
{
	public AsyncTaskFromUClass(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(
			new string[] {ModuleDirectory}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"BlueprintGraph",
				"Engine",
				"KismetCompiler",
				"PropertyEditor",
				"UnrealEd",
				"Kismet"
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			}
		);

		if (Target.bBuildEditor == true)
		{
		}

		DynamicallyLoadedModuleNames.AddRange(
			new string[] { }
		);
	}
}