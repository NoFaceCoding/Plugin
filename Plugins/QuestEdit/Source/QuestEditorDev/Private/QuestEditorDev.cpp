//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once


#include "QuestEditorDev.h"
#include "AssetToolsModule.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

//Game widgets factory
#include "Factory/AssetTypeActions_GameWidgets.h"
#include "Factory/QuestFactory_GameWidgets.h"

//Editor widgets factory
#include "Factory/AssetTypeActions_QuestEditorWidgets.h"
#include "Factory/QuestFactory_QuestEditorWidgets.h"

//Tasks factory
#include "Factory/AssetTypeActions_QuestTasks.h"
#include "Factory/QuestFactory_QuestTasks.h"

//Tasks Errors factory
#include "Factory/AssetTypeActions_QuestTasksErrors.h"
#include "Factory/QuestFactory_QuestTasksErrors.h"

//Settings
#include "QuestEditorSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"

//Placement mode
#include "IPlacementModeModule.h"  
#include "EngineUtils.h"

//Subregister function.
#include "Modules/ModuleManager.h"
#include "EditorUtilitySubsystem.h"
#include "IBlutilityModule.h"



#define LOCTEXT_NAMESPACE "QuestEditorDev"

DEFINE_LOG_CATEGORY(QuestEditorDevLog);

void QuestEditorDev::StartupModule()
{

	//----------------------------------------------------------------------
	//-------------------------------------Settings
	//----------------------------------------------------------------------

	ModuleSettings = NewObject<UQuestEditorSettings>(GetTransientPackage(), "QuestEditorSettings", RF_Standalone);
	ModuleSettings->AddToRoot();

	// Register settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "QuestEditor",
			LOCTEXT("RuntimeSettingsName", "QuestEditor"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Quest Editor Plugin settings"),
			ModuleSettings);
	}

	// Register widget to load on start
	RegisterQuestEditorWidget();

	//----------------------------------------------------------------------
	//----------------------------------Asset registry----------------------
	//----------------------------------------------------------------------
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	//-------------------------------Category reg
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	MY_QuestCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Quest")), LOCTEXT("Quest", "Quest"));
	
	//----------------------------------------------------------------------
	//-------------------------------Register asset type actions
	//----------------------------------------------------------------------

	//-------------------------------Base Quest task

	//TSharedRef<IAssetTypeActions> UBaseTaskBlueprint = MakeShareable(new AssetActionTypeBaseTask);
	//AssetTools.RegisterAssetTypeActions(UBaseTaskBlueprint);
	//
	////-------------------------------Base NPC REWARD  quest task

	//TSharedRef<IAssetTypeActions> UBaseNPCRewardTaskBlueprint = MakeShareable(new AssetActionTypeBaseNPCRewardTask);
	//AssetTools.RegisterAssetTypeActions(UBaseNPCRewardTaskBlueprint);


	////-------------------------------Base Quest Task Error

	//TSharedRef<IAssetTypeActions> UQuestEditorTaskErrorBlueprintFactory = MakeShareable(new AssetActionTypeTaskError);
	//AssetTools.RegisterAssetTypeActions(UQuestEditorTaskErrorBlueprintFactory);

	////-------------------------------Base Quest Talk Task Error

	//TSharedRef<IAssetTypeActions> UQuestEditorTalkTaskErrorBlueprintFactory = MakeShareable(new AssetActionTypeTalkTaskError);
	//AssetTools.RegisterAssetTypeActions(UQuestEditorTalkTaskErrorBlueprintFactory);

	//-------------------------------Game widgets

	TSharedRef<IAssetTypeActions> UBaseQuestGameWidgetBlueprint = MakeShareable(new AssetTypeActions_GameWidgets);
	AssetTools.RegisterAssetTypeActions(UBaseQuestGameWidgetBlueprint);

	//-------------------------------Quest Editor widgets

	TSharedRef<IAssetTypeActions> UBaseQuestEditorWidgetBlueprint = MakeShareable(new AssetTypeActions_QuestEditorWidgets);
	AssetTools.RegisterAssetTypeActions(UBaseQuestEditorWidgetBlueprint);

	//-------------------------------Quest Tasks

	TSharedRef<IAssetTypeActions> UBaseTaskBlueprint = MakeShareable(new AssetTypeActions_QuestTasks);
	AssetTools.RegisterAssetTypeActions(UBaseTaskBlueprint);

	//-------------------------------Quest Tasks Errors

	TSharedRef<IAssetTypeActions> UQuestEditorTaskErrorBlueprint = MakeShareable(new AssetTypeActions_QuestTasksErrors);
	AssetTools.RegisterAssetTypeActions(UQuestEditorTaskErrorBlueprint);


	//----------------------------------------------------------------------
	//-----------------------------------Icons
	//----------------------------------------------------------------------
	StyleSet = MakeShareable(new FSlateStyleSet("QuestTaskStyle"));

	//Content path of this plugin
	FString ContentDir = IPluginManager::Get().FindPlugin("QuestEditor")->GetBaseDir();

	//The image we wish to load is located inside the Resources folder inside the Base Directory
	//so let's set the content dir to the base dir and manually switch to the Resources folder:
	StyleSet->SetContentRoot(ContentDir);

	//Create a brush from the icon
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/QuestTaskIcon"), TEXT(".png")), FVector2D(128.f, 128.f));

	if (ThumbnailBrush)
	{
		//In order to bind the thumbnail to our class we need to type ClassThumbnail.X where X is the name of the C++ class of the asset
		StyleSet->Set("ClassThumbnail.BaseQuestTask", ThumbnailBrush);

		//Register the created style
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}

	//----------------------------------------------------------------------
	//---------------------------------- PLACEMENT MODULE
	//----------------------------------------------------------------------
	
	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	{
		//Register cat
		PlacementModeModule.RegisterPlacementCategory(FPlacementCategoryInfo(
			NSLOCTEXT("PlacementMode", "Quest", "Quest"),
			"Quest",
			TEXT("Quest"),
			40
		));

		//Add assets to category
		//PlacementModeModule.RegisterPlaceableItem("Quest", MakeShareable(new FPlaceableItem(*UQuestLocationSphereFactory::StaticClass())));

		TArray<UObject*> tempArray;
		if (EngineUtils::FindOrLoadAssetsByPath("/QuestEditor/QuestLocations", tempArray, EngineUtils::ATL_Class))
		{
			for (int i = 0; i < tempArray.Num(); ++i)
			{				
				UObject* temp = tempArray[i];
				if (temp == NULL)
				{
					continue;
				}

				if (!temp->GetName().StartsWith("SKEL_") && !temp->GetName().StartsWith("REINST_") && !temp->GetName().StartsWith("PLACEHOLDER"))
				{
					//Skip the base one
					if (!temp->GetName().Contains("BP_BaseQuestLocationActor"))
					{
						IPlacementModeModule::Get().RegisterPlaceableItem("Quest", MakeShareable(new FPlaceableItem(nullptr, FAssetData(temp))));
					}					
				}
			}
		}
	}
}

void QuestEditorDev::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	//----------------------------------------------------------------------
	//-----------------------------------Settings
	//----------------------------------------------------------------------
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "QuestEditor");
	}

	if (!GExitPurge)
	{
		ModuleSettings->RemoveFromRoot();
	}
	else
	{
		ModuleSettings = nullptr;
	}

	//----------------------------------------------------------------------
	//-----------------------------------Icons
	//----------------------------------------------------------------------
	//Unregister the style
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());
	
	//----------------------------------------------------------------------
	//---------------------------------- PLACEMENT MODULE
	//----------------------------------------------------------------------
	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	{
		//if (PlacementModule)
		{
			PlacementModeModule.UnregisterPlacementCategory("Quest");;
		}
	}

}

UQuestEditorSettings* QuestEditorDev::GetSettings() const
{
	check(ModuleSettings);
	return ModuleSettings;
}

void QuestEditorDev::RegisterQuestEditorWidget()
{
	//find config.
	if (!GConfig)
	{
		UE_LOG(QuestEditorDevLog, Warning, TEXT("UQuestEditorDev::RegisterQuestEditorWidget - Invalid config module."));
		return;
	}		
	
	//------------------------------Read config
	
	FString Section = "/Script/Blutility.EditorUtilitySubsystem";
	FString Key = "+LoadedUIs";
	
	TArray <FString> Values;

	//Get array of loaded UIs.
	FString ConfigFile = FPaths::ProjectConfigDir() + "DefaultEditorPerProjectUserSettings.ini";

	GConfig->GetArray(*Section, *Key, Values, ConfigFile);
	
	bool IsRegisted = false;

	for (int i = 0; i < Values.Num(); i++)
	{
		//Quest editor is already in the config?
		if (Values[i] == "/QuestEditor/QuestEditor/QuestEditor.QuestEditor")
		{
			IsRegisted = true;
			break;
		}
	}
	//---------------------------If is not registerd, add it to config.		
	if (!IsRegisted)
	{
		//Add to array
		Values.Add(TEXT("/QuestEditor/QuestEditor/QuestEditor.QuestEditor"));
		//Set array in config
		GConfig->SetArray(*Section, *Key, Values, ConfigFile);
		//Save config file
		GConfig->Flush(false, ConfigFile);
		UE_LOG(QuestEditorDevLog, Log, TEXT("UQuestEditorDev::RegisterQuestEditorWidget - Added Quest Editor Widget to load on startup."));	
		
		//Manually load the editor widget the first time
		SubRegisterQuestEditorWidget();
	}
}

void QuestEditorDev::SubRegisterQuestEditorWidget()
{
	FSoftObjectPath BlueprintPath = FSoftObjectPath("/QuestEditor/QuestEditor/QuestEditor.QuestEditor");
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	EditorUtilitySubsystem->LoadedUIs.AddUnique(BlueprintPath);
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(QuestEditorDev, QuestEditorDev)