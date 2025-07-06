//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

using UnrealBuildTool;

public class QuestEditorDev : ModuleRules
{
    public QuestEditorDev(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...  
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...                               
             
            }
            );

       //PrivateIncludePaths.Add("QuestEditorDev/Private");
       // PublicIncludePaths.Add("QuestEditorDev/Public");

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
				// ... add other public dependencies that you statically link with here ...
                "InputCore",
            }
            );

        //Only editor modules
        if (Target.Type == TargetRules.TargetType.Editor)
        {
        
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            "UnrealEd",
            "UMG",
            "UMGEditor",
            "Blutility",
            "Kismet",
            "ToolMenus",
            "PlacementMode",
            "CoreUObject",
            }
            );
        }

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Engine",
                "EditorSubsystem",
                "KismetCompiler",
                "QuestEditor",
                "Slate",
                "SlateCore",  
                "Projects",
                "GameplayTags",
                "AssetTools",
                "AssetRegistry",
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
