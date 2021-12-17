// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectS2 : ModuleRules
{
	public ProjectS2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
