// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DDD : ModuleRules
{
	public DDD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Public/Private 구조 없이 모듈 루트(Source/DDD/)를 include path로 추가
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"GameplayAbilities", "GameplayTags", "GameplayTasks",
			"UMG",      // UUserWidget, UWidget
			"Slate",    // SVirtualJoystick
			"SlateCore"
		});
	}
}
