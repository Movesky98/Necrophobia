// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Necrophobia : ModuleRules
{
	public Necrophobia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
