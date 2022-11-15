// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pro4 : ModuleRules
{
	public Pro4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"UMG",
			"OnlineSubsystem",
			"OnlineSubsystemNULL",
			"OnlineSubSystemSteam",
			"AIModule",
			"NavigationSystem",
			"GameplayTasks",
			"Paper2D"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate", 
			"SlateCore" 
		});
	}
}
