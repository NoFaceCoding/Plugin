//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "QuestEditor/QuestTypesDev.h"

#include "QuestEditorSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class QUESTEDITORDEV_API UQuestEditorSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	//-----------------------------------------------------------------------
	//-------------------------------------Quest Editor Content
	//-----------------------------------------------------------------------

	/**
	*	If you want to include the example quests, dialogs and npcs in the quest editor.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Content")
		bool ShowExampleContent;

	/**
	*	Path for all the folders that contains datatables for quest editor.
	*	This includes quest, dialogs, npcdef, taskinfo and talktaskinfo.
	*	Try to keep this folders with only this content and the path to be as precise as possible. This will improve the performance when searching for them.
	*	If a folder is invalid it wil crash the editor.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Content")
		TArray <FString> DatatablesPaths;

	//-----------------------------------------------------------------------
	//-------------------------------------Quest Editor
	//-----------------------------------------------------------------------

	/**
	*	The talk mode name that will be used by default in the Quest Editor. If invalid, conversation mode will be used. 
	*	If there are two modes with the same name, the first will be used.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		FString DefaultTalkMode;

	/**
	*	The editor invisible grid size. This is used when aligning nodes.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		FVector2D EditorGridDimensions;

	/**
	*	The details panel size in pixels. 
	*	The width will determine the scale. 
	*	The height will determine when the scrollbar will be shown.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		FVector2D DetailsPanelSize;

	/**
	*	How much sensitivity in units should be used by each arrow key press.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		float ArrowsSensitivity;

	/**
	*	How much sensitivity the scrolling wheel zoom should have.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		float ZoomSensitivity;

	/**
	*	Autosave frequency for the editor in seconds.
	*	If set to 0, it will disable the editor autosaving.
	*	If less than 30 seconds, it will get set to 30 seconds, to avoid too frequent autosaves.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		float AutosaveFrequency;

	/**
	*	Use the NPC name as the rowname for the datatable instead of generating one.
	*	WARNING - This might generate problems if using two names that are equal and potentially lose data. 
	*	WARNING - The user must ensure the names provided are eligible for datatable row name.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		bool UseNPCNameAsRowName;

	/**
	*	Use the Quest name as the rowname for the datatable instead of generating one.
	*	WARNING - This might generate problems if using two names that are equal and potentially lose data.
	*	WARNING - The user must ensure the names provided are eligible for datatable row name.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor")
		bool UseQuestNameAsRowName;

	//-----------------------------------------------------------------------
	//-------------------------------------Quest Editor Widgets
	//-----------------------------------------------------------------------

	/**
	*	Selector to use in the editor for NPCs.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Widgets")
		FString EditorNPCSelector;

	/**
	*	Selector to use in the editor for quests.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Widgets")
		FString EditorQuestSelector;

	/**
	*	Selector to use in the editor for tasks objects.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Widgets")
		FString EditorTaskSelector;


	//-----------------------------------------------------------------------
	//-------------------------------------Style
	//-----------------------------------------------------------------------

	/**
	*	Color for the task nodes background.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Style")
		FLinearColor TasksBackgroundColor;

	/**
	*	Color for the talk task nodes background.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Style")
		FLinearColor TalkTasksBackgroundColor;

	/**
	*	How big the arrows should be draw in the editor.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Style")
		float ArrowSize;

	/**
	*	How thick the lines should be draw in the editor.
	*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Style")
		float LineThickness;

	//-----------------------------------------------------------------------
	//-------------------------------------Shortcuts
	//-----------------------------------------------------------------------

	/** Shortcut key bindings that are used inside the QuestEditor.*/
	UPROPERTY(Config, EditAnywhere, Category = "Quest Editor Shortcuts")
		TArray<FQuestEditorKeyBind> QuestEditorShortcuts;
	

	//-----------------------------------------------------------------------
	//-------------------------------------Editor version control
	//-----------------------------------------------------------------------
	
	/** Last version of the editor that was used to handle Post-Update Changes */
	UPROPERTY(Config)
		int PreviousQEVersion;

	UFUNCTION()
		void SetQEVersion(int NewVersion);
};
