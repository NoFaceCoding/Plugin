//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "QuestEditorSettings.h"

#include "QuestEditor/QuestEditorDevFunctionLibrary.h"



UQuestEditorSettings::UQuestEditorSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultTalkMode = "Conversation";
	ShowExampleContent = true;
	DatatablesPaths = TArray <FString>();

	EditorGridDimensions = { 200,100 };
	DetailsPanelSize = { 500,1000 };
	AutosaveFrequency = 60;
	ArrowsSensitivity = 10;
	ZoomSensitivity = 0.05;
	UseNPCNameAsRowName = false;
	UseQuestNameAsRowName = false;

	EditorNPCSelector = "/QuestEditor/QuestEditor/Selectors/StringSelectors/EWBP_NPCSelector.EWBP_NPCSelector_C";
	EditorQuestSelector = "/QuestEditor/QuestEditor/Selectors/StringSelectors/EWBP_QuestSelector.EWBP_QuestSelector_C";
	EditorTaskSelector = "/QuestEditor/QuestEditor/Selectors/TaskSelector/EWBP_TaskSelector.EWBP_TaskSelector_C";

	TasksBackgroundColor = { 0.06301,0.06301,0.06301,1 };
	TalkTasksBackgroundColor = { 0.06301,0.06301,0.06301,1 };
	ArrowSize = 1;
	LineThickness = 2;

	QuestEditorShortcuts = UQuestEditorDevFunctionLibrary::GetDefaultQuestEditorKeybinds();

	PreviousQEVersion = -1;
}

void UQuestEditorSettings::SetQEVersion(int NewVersion)
{
	PreviousQEVersion = NewVersion;
	
	//find config.
	if (!GConfig)
	{
		return;
	}

	//Get array of loaded UIs.
	FString ConfigFile = FPaths::ProjectConfigDir() + "DefaultEngine.ini";

	//------------------------------Handle config manually
	FString Section = "/Script/QuestEditorDev.QuestEditorSettings";
	FString Key = "PreviousQEVersion";
		
	//Set in config
	GConfig->SetInt(*Section, *Key, NewVersion, ConfigFile);
			
	//Save config file
	GConfig->Flush(false, ConfigFile);
}
