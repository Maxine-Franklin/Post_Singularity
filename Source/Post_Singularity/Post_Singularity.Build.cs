// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Post_Singularity : ModuleRules
{
	public Post_Singularity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
