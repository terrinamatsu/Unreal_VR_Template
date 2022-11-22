// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PipelineTD_01 : ModuleRules
{
	public PipelineTD_01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "UnrealEd", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "ProceduralMeshComponent", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
