// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Necrophobia : ModuleRules
{
	public Necrophobia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
        });

		PrivateIncludePaths.AddRange(new string[] {
			"Necrophobia",
			"Necrophobia/Session",
			"Necrophobia/GameFramwork",
			"Necrophobia/ZombieEscape",
		});
	}
}
