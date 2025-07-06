//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Quest/QuestTypes.h"
#include "QuestEditor/QuestTypesDev.h"
#include "UObject/SoftObjectPath.h"
#include "BaseQuestEditor.generated.h"

/**
 *	Base quest editor widget 
 */

class UBaseTooltipContentWidget;
class UQuestEditorSaveGame;

 //------------------------------------------------------------------------------------------------
 //-----------------------------------------------Dispatchers
 //------------------------------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideTooltip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FShowTooltip, const FQuestTaskPayload&, PayloadIn, TSubclassOf<UBaseTooltipContentWidget>, TooltipContentWidget, bool, UseMousePosition, FVector2D, Offset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShowMessage, const FString&, StringIn, const FLinearColor&, ColorIn);

UCLASS(Config = Game)
class QUESTEDITORDEV_API UBaseQuestEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()
			   		 	  
		
public:

	UBaseQuestEditor();

	virtual void NativeConstruct() override;

	//------------------------------------------------------------------------------------------------
	//-----------------------------------------------Dispatchers
	//------------------------------------------------------------------------------------------------

	/*Call tooltip event to hide tooltip on quest editor*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable,  Category = "Tooltip")
		FHideTooltip HideTooltip;

	/**
	*	Call tooltip event to show tooltip on quest editor
	*	Payload is passed to update the tooltip widget.
	*	Class is passed for the tooltip content widget.
	*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Tooltip")
		FShowTooltip ShowTooltip;

	/**
	*	Call when want to show a message in ui.
	*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Messages")
		FShowMessage ShowMessage;

	/*Show the changelog inside the editor*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Messages")
		FHideTooltip ShowChangelog;  //hide tooltip has no variables.	

	//------------------------------------------------------------------------------------------------
	//------------------------------------------------Variables
	//------------------------------------------------------------------------------------------------

	/**
	*	All datatables of quest struct that are in the specified folder.
	*	This are determined in editor begin play.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Quest Datatables")
		TArray <UDataTable*> QuestDatatables;

	/**
	*	All datatables of dialog struct that are in the specified folder.
	*	This are determined in editor begin play.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Quest Datatables")
		TArray <UDataTable*> DialogDatatables;

	/**
	*	All datatables of NPCQuestDef struct that are in the specified folder.
	*	This are determined in editor begin play.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Quest Datatables")
		TArray <UDataTable*> NPCDatatables;

	/**
	*	All datatables of QuestTaskEditorInfo struct that are in the specified folder.
	*	This are determined in editor begin play.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Quest Datatables")
		TArray <UDataTable*> TaskInfoDatatables;

	/**
	*	All datatables of QuestTalkTaskEditorInfo struct that are in the specified folder.
	*	This are determined in editor begin play.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Quest Datatables")
		TArray <UDataTable*> TalkTaskInfoDatatables;

	/**
	*	Standar color for normal messages in the editor
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Messages")
		FLinearColor EditorNormalMessageColor = {.7,.7,.7,1};
	
	/**
	*	Standar color for warnning messages in the editor
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Messages")
		FLinearColor EditorWarnningMessageColor = { .6,.6,.3,1 };

	//------------------------------------------------------------------------------------------------
	//------------------------------------------------FUNCTIONS
	//------------------------------------------------------------------------------------------------
	//---------------------------------------------Datatables
private:
	/**
	*	Sets all datatables array for further using in child widgets that have a reference to this class.
	*	This ensures its only done once for performance reasons.
	*	Called on construct.
	*/
	UFUNCTION()
		void SetAllDatatablesOfQuest();

	/**
	*	Appends all datatables found in the folder path to the corresponding array.
	*/
	UFUNCTION()
		void AppendDatatablesOfQuestForOneFolderPath(const FString& FolderPath);
	
	/**
	*	Fixes all datatables saved with update data for current version.
	*/
	UFUNCTION()
		bool FixPreviousVersionData(FString& MessageOut,bool& ShowChangelogOut);

	/**
	*	Converts Dialogs dts to quest and fixes localization data
	*/
	UFUNCTION()
		bool ChangeDTsType();

	/**
	*	Delete editor save game
	*/
	UFUNCTION()
		bool DeleteQuestEditorSaveGame();

	/**
	*	Gets the current selector for the editor for tasks for any class.
	*	In case of fail load, must provide a default selector.
	*/
	UFUNCTION()
		UClass* GetSelectorFromClass(FSoftClassPath ClassSelected, FSoftClassPath DefaultSelector);

	/**
	*	Gets the current selector for the editor for tasks.
	*	This can be set in the config file.
	*	Used to created Quest Editor UI.
	*	If is not set correctly, will use default instead.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Settings")
		UClass* GetQETaskSelector();

	/**
	*	Gets the current selector for the editor for Quests.
	*	This can be set in the config file.
	*	Used to created Quest Editor UI.
	*	If is not set correctly, will use default instead.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Settings")
		UClass* GetQEQuestSelector();

	/**
	*	Gets the current selector for the editor for tasks.
	*	This can be set in the config file.
	*	Used to created Quest Editor UI.
	*	If is not set correctly, will use default instead.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Settings")
		UClass* GetQENPCSelector();
	
	/**
	*	Gets the editor grid size.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Settings")
		FVector2D GetEditorGridSize();
	
	/**
	*	Gets the editor autosave frequency.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Settings")
		float GetEditorAutosaveFreq();
	

	//---------------------------------------------Editor

	/**
	*	Gets the current default talk mode for the quest editor.
	*	This can be set in the project settings.
	*	If the mode set in settings is invalid, this will return a null struct.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Settings")
		FQuestTalkTaskEditorInfo GetDefaultTalkModeInfo(bool& FoundModeOut);

	/**
	*	Calls delegate to show message with normal color.
	*/
	UFUNCTION(BlueprintCallable, Category = "Messages")
		void ShowNormalMessage(const FString& MessageIn);

	/**
	*	Calls delegate to show message with warnning color.
	*/
	UFUNCTION(BlueprintCallable, Category = "Messages")
		void ShowWarnningMessage(const FString& MessageIn);

	//---------------------------------------------Talk Modes
	/**
	*	Gets the current talk mode for the quest editor.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Settings")
		FQuestTalkTaskEditorInfo GetCurrentTalkModeInfo(const FQuest& QuestIn);


	//---------------------------------------------Saving and loading
	/*Get savegame object for quest editor*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-SaveGame")
		UQuestEditorSaveGame* QEditorSave_GetSavegame(bool& FoundSaveOut);

	/*Save all variables into a savegame object for the quest editor*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-SaveGame")
		void QEditorSave_SaveVariables(bool MantainPrevDialogsIn, bool MantainPrevQuestsIn, EQuestEditor_EditMode CurrentEditorModeIn, int UndoQuestIndexSavedIn, int UndoDialogIndexSavedIn, float ViewportScaleSavedIn,
		const TArray<FQuest>& PreviousQuestsIn, const TArray<FQuest>& UndoHistoryQuestsIn, const TArray<FQuest>& PreviousDialogsIn, const TArray<FQuest>& UndoHistoryDialogsIn);

	/*Get all variables from the savegame object for the quest editor*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-SaveGame")
		void QEditorSave_GetVariables(EQuestEditor_EditMode& CurrentEditorModeOut, int& UndoQuestIndexSavedOut, int& UndoDialogIndexSavedOut, float& ViewportScaleSavedOut,
			TArray<FQuest>& PreviousQuestsOut, TArray<FQuest>& UndoHistoryQuestsOut, TArray<FQuest>& PreviousDialogsOut, TArray<FQuest>& UndoHistoryDialogsOut);

	/**
	*	Get quests and dialogs arrays from the savegame object for the quest editor.
	*	Returns wether or not the savegame was found.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-SaveGame")
		bool QEditorSave_GetVariables_Primary(TArray<FQuest>& PreviousQuestsOut, TArray<FQuest>& PreviousDialogsOut);

	/**
	*	Get quests and dialogs arrays from the savegame object for the quest editor.
	*	Returns wether or not the savegame was found.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-SaveGame")
		bool QEditorSave_GetVariables_Undo(int& UndoQuestIndexSavedOut, int& UndoDialogIndexSavedOut, TArray<FQuest>& UndoHistoryQuestsOut, TArray<FQuest>& UndoHistoryDialogsOut);

	/**
	*	Get misc variables from the savegame object for the quest editor.
	*	Returns wether or not the savegame was found.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-SaveGame")
		bool QEditorSave_GetVariables_Misc(EQuestEditor_EditMode& CurrentEditorModeOut, float& ViewportScaleSavedOut);

protected:

	//---------------------------------------------Editor Updating
	//--------------------Quest talk task
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-TalkTask")
		void UpdateTalkTaskNPC(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, const FNPCQuestDefRep& NPCDefIn, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-TalkTask")
		void UpdateTalkTaskShortDesc(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, FText ShortDescIn, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-TalkTask")
		void UpdateTalkTaskCustomPayloadData(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, FString CustomPaylodDataIn, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-TalkTask")
		void UpdateTalkTaskYouSay(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, const FText& YouSay, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-TalkTask")
		void UpdateTalkTaskNPCResponse(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, FText NpcResponse, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-TalkTask")
		void UpdateTalkTaskPlayerCue(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, TSoftObjectPtr<USoundCue> PlayerCue, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-TalkTask")
		void UpdateTalkTaskNPCCue(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TalkTasksIn, TSoftObjectPtr<USoundCue> NpcCue, TArray <FQuestTaskID>& TalkTaskIDsOut, TArray <FQuestTalkTask>& TalkTasksOut);

	//--------------------Quest Task

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskNameInv(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FString NameIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskName2Inv(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FString NameIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskTimesToCheck(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, int TimesIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskAmountInv(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, float AmountIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskTagsRequired(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FGameplayTagContainer TagsIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskTagsIgnored(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FGameplayTagContainer TagsIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskShortDesc(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FText ShortDescIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateInverseActivateCond(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, bool InverseCondIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskClass(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, TSoftClassPtr<UObject> ClassIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskVector(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FVector VectorIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskPayload(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, FQuestTaskPayload PayloadIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskSoftClass(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, TSoftClassPtr<UBaseQuestTask> TaskClassIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);

	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Task")
		void UpdateTaskCustomPayload(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& TasksIn, int PayloadSlotIn, FString PayloadDataIn, TArray <FQuestTaskID>& TaskIDsOut, TArray <FQuestTask>& TasksOut);
	
	//------------------------------NODES Position

	/*Update Quest editor position in struct. This is the position where the node is placed in the editor.*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool UpdateNodeQEPosition(UPARAM(ref)FQuest& QuestIn, FQuestTaskID TaskID, FVector2D NewPositionIn);

	/**
	*	Aligns nodes to side of the selected nodes
	*	@Param UseY If false use X, if true use Y.
	*	@Param CheckLower If false use Higher, if true use Lower.
	*	Returns all positions vectors.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FVector2D> AlignNodesToSides(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, bool UseY, bool CheckLower);
		   
	/**
	*	Creates a grid based on the parameters and adjust all nodes positions to the it.
	*	Returns all positions vectors.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FVector2D> AlignNodesToGrid(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, const FVector2D& GridSizeIn);

	/**
	*	Takes the nodes with lower and higher position, and fills all other nodes evenly in between.
	*	At least three nodes must be provided.
	*	It can also align nodes.
	*	Returns all positions vectors.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FVector2D> AlignNodesToEvenDistribution(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn,  bool UseY);

	/**
	*	Adds a amount to a node position. This allows to displace the nodes.
	*	Useful for making nodes move with arrow keys.
	*	Returns all positions vectors.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static TArray <FVector2D> DisplaceNodesPosition(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn, float Displacement, bool UseY, bool UseInverseOperation);
	
	/**
	*	Connect all selected nodes based on position.
	*	Higher nodes are superior nodes of lower nodes. Only one connection per node is created (Chain)

	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void ConnectNodes(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn);

	/**
	*	Connect all selected nodes based on position in grid (Y axis). 
	*	Nodes with same grid position wont be connected.
	*	Nodes with immediate inferior or superior grid position will be connected with the corresponding links.
	*	Must provide the node size and grid size to be consistent with the visual grid.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void ConnectNodesGridLayout(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, const FVector2D& GridSizeIn);
	
	/**
	*	Connect all selected nodes in a mutually exclusively disable connection.
	*	Each node will disable all the others
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void ConnectNodesDisabling(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn);

	/**
	*	Removes all normal connections (not disabling ones) of selected nodes
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void RemoveSelectedNodesConnections(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn);

	/**
	*	Removes all disable connections of selected nodes
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void RemoveSelectedNodesDisableConnections(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn);

	/**
	*	Toggles all selected nodes connection type
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void ToggleSelectedNodesConnectionsType(UPARAM(ref)FQuest& QuestIn, const TArray <FQuestTaskID>& SelectedNodesIn);

	//------------------------------NODES Operations
	
	/**
	*	Swaps all connections of NodeA with NodeB.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		void SwapNodeConnectionsAndPrintErrors(UPARAM(ref)FQuest& QuestIn, const FQuestTaskID& NodeA, const FQuestTaskID& NodeB, ENodeConnectorType ConnectorIn);

	//------------------------------NODES Copy Paste

	/**
	*	Handles all the operation for copy pasting nodes inside the Quest Editor.
	*	@PARAM QuestIn Quest to modify in Editor.
	*	@PARAM QuestCopiedIn Quest at the moment of copying the nodes. Used to mantain the state of the quest from when the nodes where copied and not the possible modified current state.
	*	@PARAM CopiedNodesIn Nodes selected at the moment of copy.
	*	@PARAM MousePastePositionIn Mouse position when pasting nodes.
	*	@PARAM PastedNodesOut New nodes IDS added to the quest as result of the paste operation.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static void HandleCopyPasteNodes(UPARAM(ref)FQuest& QuestIn, const FQuest& QuestCopiedIn, const TArray <FQuestTaskID>& CopiedNodesIn, const TArray <FVector2D>& NodesWidgetSizeIn, FVector2D MousePastePositionIn, TArray <FQuestTaskID>& PastedNodesOut);

	/** 
	*	Clear NPC for al talk tasks of quest.
	*	Used after pasting nodes in dialog mode.
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestEditor-Node")
		static bool ClearNPCForAllTalkTasks(UPARAM(ref)FQuest& QuestIn);





};
